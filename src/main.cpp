#include "./arp/arp_hdr.hpp"
#include "./tap/tap.hpp"
#include "arp/arp_ipv4.hpp"
#include "ethernet/ethernet.hpp"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdint>
using namespace std;

constexpr int BUF_LEN = 100;
constexpr uint16_t IPV4_TYPE = 0x0800;
constexpr uint16_t ARP_TYPE = 0x0806;
constexpr uint16_t ETH_HW_TYPE = 0x0001;

int main() {
    cout << "Hi!";

    char dev_name[TAP_NAME_SIZE] = "tap0";
    tap_device* dev = new tap_device(dev_name);

    uint8_t buf[BUF_LEN];

    // attempt to ARP
    eth_hdr* hdr;

    while (true) {
        ssize_t res = dev->read_tap(buf, BUF_LEN, hdr);

        if (res <= 0) {
            cout << "read error\n";
            break;
        }

        // attempt to convert wire data to ethernet
        hdr = (eth_hdr*)buf;

        const uint16_t ethertype = ntohs(hdr->get_ethertype());

        if (ethertype == IPV4_TYPE) {
            cout << "IPv4 packet Incoming! \n";

            // do nothing

        } else if (ethertype == ARP_TYPE) {
            cout << "ARP Packet Incoming!\n";

            // attempt to convert payload into arp header

            arp_hdr* in_arp_hdr = (arp_hdr*)hdr->get_payload();

            // get arp packet inside the header

            arp_ipv4* pac = (arp_ipv4*)in_arp_hdr->get_payload();

            cout << pac->to_string();

            // implement arp algorithm

            // ?Do I have the hardware type in ar_hdr;
            bool hw_type = ntohs(in_arp_hdr->get_hw_type());
            cout << "hardware type: " << hw_type << "\n";

            if (hw_type == ETH_HW_TYPE) {
                // Yes: (almost definitely)
                //   [optionally check the hardware length ar_hln]
                uint8_t hw_size = in_arp_hdr->get_hw_size();
                cout << "hardware size: " << hw_size << "\n";

                //   ?Do I speak the protocol in ar_pro?

                uint16_t pro_type = ntohs(in_arp_hdr->get_pro_type());
                cout << "protocol type: " << pro_type << "\n";

                if (pro_type == IPV4_TYPE) {

                    //   Yes:
                    //     [optionally check the protocol length ar_pln]
                    uint8_t pro_size = in_arp_hdr->get_pro_size();
                    cout << "protocol size: " << pro_size << "\n";

                    //     Merge_flag := false
                    bool merge_flag = false;
                }
            }

            //     If the pair <protocol type, sender protocol address>
            //     is
            //         already in my translation table, update the
            //         sender hardware address field of the entry with
            //         the new information in the packet and set
            //         Merge_flag to true.
            //     ?Am I the target protocol address?
            //     Yes:
            //       If Merge_flag is false, add the triplet <protocol
            //       type,
            //           sender protocol address, sender hardware
            //           address> to the translation table.
            //       ?Is the opcode ares_op_REQUEST?  (NOW look at the
            //       opcode!!) Yes:
            //         Swap hardware and protocol fields, putting the
            //         local
            //             hardware and protocol addresses in the sender
            //             fields.
            //         Set the ar_op field to ares_op$REPLY
            //         Send the packet to the (new) target hardware
            //         address on
            //             the same hardware on which the request was
            //             received.
            //

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
