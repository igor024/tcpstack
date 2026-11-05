#include "./arp/arp_hdr.hpp"
#include "./tap/tap.hpp"
#include "arp/arp_ipv4.hpp"
#include "ethernet/ethernet.hpp"
#include "util/util.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <system_error>
using namespace std;

constexpr int BUF_LEN = 100;
constexpr uint16_t IPV4_TYPE = 0x0800;
constexpr uint16_t ARP_TYPE = 0x0806;
constexpr uint16_t ETH_HW_TYPE = 0x0001;
constexpr uint16_t CACHE_LEN = 0x0010;
constexpr uint16_t ARP_REQ = 0x0001;
constexpr uint16_t ARP_REP = 0x0002;

constexpr size_t frame_size =
    sizeof(eth_hdr) + sizeof(arp_hdr) + sizeof(arp_ipv4);

struct trans_entry {
        uint16_t hw_type;
        uint8_t s_mac[6];
        uint16_t pro_type;
        uint32_t s_ip;
};

trans_entry trans_table[CACHE_LEN];
uint16_t trans_table_idx = 0;

int main() {
    srand(time(NULL));

    // create dummy info

    uint8_t my_mac[6];
    util::rand_mac(my_mac);

    uint32_t my_ip = util::create_ip(10, 0, 0, 4);

    char dev_name[TAP_NAME_SIZE] = "tap0";
    tap_device* dev = new tap_device(dev_name);

    uint8_t buf[BUF_LEN];

    // attempt to ARP
    eth_hdr* in_eth;

    while (true) {
        ssize_t res = dev->read_tap(buf, BUF_LEN, in_eth);

        if (res <= 0) {
            cout << "read error\n";
            break;
        }

        // attempt to convert wire data to ethernet
        in_eth = (eth_hdr*)buf;

        const uint16_t ethertype = ntohs(in_eth->get_ethertype());

        if (ethertype == IPV4_TYPE) {
            cout << "IPv4 packet Incoming! \n";

            // do nothing

        } else if (ethertype == ARP_TYPE) {
            cout << "ARP Packet Incoming!\n";

            // attempt to convert payload into arp header

            arp_hdr* in_arp = (arp_hdr*)in_eth->get_payload();

            // get arp packet inside the header

            arp_ipv4* in_ipv4 = (arp_ipv4*)in_arp->get_payload();

            cout << in_ipv4->to_string();

            // implement arp algorithm

            // ?Do I have the hardware type in ar_hdr;
            uint16_t hw_type = ntohs(in_arp->get_hw_type());
            cout << "hardware type: " << hw_type << "\n";

            if (hw_type == ETH_HW_TYPE) {
                // Yes: (almost definitely)
                //   [optionally check the hardware length ar_hln]
                uint8_t hw_size = in_arp->get_hw_size();
                cout << "hardware size: " << (int)hw_size << "\n";

                //   ?Do I speak the protocol in ar_pro?

                uint16_t pro_type = ntohs(in_arp->get_pro_type());
                cout << "protocol type: " << pro_type << "\n";

                if (pro_type == IPV4_TYPE) {

                    //   Yes:
                    //     [optionally check the protocol length ar_pln]
                    uint8_t pro_size = in_arp->get_pro_size();
                    cout << "protocol size: " << (int)pro_size << "\n";

                    //     Merge_flag := false
                    bool merge_flag = false;

                    //     If the pair <protocol type, sender protocol address>
                    //     is
                    //         already in my translation table, update the
                    //         sender hardware address field of the entry with
                    //         the new information in the packet and set
                    //         Merge_flag to true.
                    for (int i = 0; i < trans_table_idx; ++i) {
                        trans_entry& t = trans_table[i];

                        if (t.pro_type == pro_type && t.hw_type == hw_type &&
                            t.s_ip == in_ipv4->get_s_ip()) {

                            memcpy(t.s_mac, in_ipv4->get_s_mac(), 6);

                            merge_flag = true;

                        }
                    }

                    //     ?Am I the target protocol address?
                    uint32_t target_ip = ntohl(in_ipv4->get_d_ip());
                    if (target_ip == util::create_ip(10, 0, 0, 1)) {
                        //       If Merge_flag is false, add the triplet
                        //       <protocol type,
                        //           sender protocol address, sender hardware
                        //           address> to the translation table.
                        //       ?Is the opcode ares_op_REQUEST?  (NOW look at
                        //       the
                        if (!merge_flag && trans_table_idx != CACHE_LEN) {
                            trans_entry& t = trans_table[trans_table_idx];
                            t.hw_type = hw_type;
                            t.pro_type = pro_type;
                            t.s_ip = in_ipv4->get_s_ip();
                            memcpy(t.s_mac, in_ipv4->get_s_mac(), 6);

                            ++trans_table_idx;
                        }

                        uint16_t arp_op = ntohs(in_arp->get_op_code());
                        if (arp_op == ARP_REQ) {
                            // reply with my ip & mac

                            // set dest addresses
                            const uint8_t* dest_mac = in_ipv4->get_s_mac();
                            uint32_t dest_ip = in_ipv4->get_s_ip();

                            in_ipv4->set_d_ip(dest_ip);
                            in_ipv4->set_d_mac(dest_mac);
                            in_eth->set_dest(dest_mac);

                            // set source addresses

                            in_ipv4->set_s_ip(htonl(my_ip));

                            in_ipv4->set_s_mac(my_mac);
                            in_eth->set_source(my_mac);

                            in_arp->set_op_code(htons(ARP_REP));

                            bool success = dev->write_eth(in_eth, frame_size);

                            if (success) {
                                cout << "Successfully wrote frame\n";
                            }
                        }
                    }
                }
            }
            /*

            const uint8_t* src = hdr->get_source();

            for (int i = 0; i < 6; ++i) {
                cout << *(src + i) << " ";
            }
            */
            cout << "\n";
        }
    }
}
