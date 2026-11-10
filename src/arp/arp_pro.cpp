#include "./arp_pro.hpp"
#include "../tap/tap.hpp"
#include "../util/util.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>

using namespace std;

void arp_pro::handle_arp(
    uint32_t my_ip,
    uint8_t* my_mac,
    tap_device* dev,
    eth_hdr* in_eth
) {
    cout << "ARP Packet Incoming!\n";

    // attempt to convert payload into arp header

    arp_hdr* in_arp = (arp_hdr*)in_eth->get_payload();

    // get arp packet inside the header

    arp_ipv4* in_ipv4 = (arp_ipv4*)in_arp->get_payload();

    cout << in_ipv4->to_string();

    // do I have the hardware type
    uint16_t hw_type = ntohs(in_arp->get_hw_type());
    cout << "hardware type: " << hw_type << "\n";

    if (hw_type == util::ETH_HW_TYPE) {
        // yes: (almost definitely)
        // TODO: [optionally check the hardware length ar_hln]
        uint8_t hw_size = in_arp->get_hw_size();
        cout << "hardware size: " << (int)hw_size << "\n";

        // do I speak the protocol

        uint16_t pro_type = ntohs(in_arp->get_pro_type());
        cout << "protocol type: " << pro_type << "\n";

        if (pro_type == util::IPV4_TYPE) {

            // TODO: [optionally check the protocol length ar_pln]
            uint8_t pro_size = in_arp->get_pro_size();
            cout << "protocol size: " << (int)pro_size << "\n";

            // if the pair <protocol type, sender protocol address> exists in
            // the translation table, update (merge) it
            bool merge_flag =
                update_pair(in_ipv4->get_s_ip(), in_ipv4->get_s_mac());

            // am I the target protocol address
            uint32_t target_ip = ntohl(in_ipv4->get_d_ip());
            cout << "got here: \n";
            cout << util::format_ip(target_ip) << "\n";
            if (target_ip == my_ip) {
                // add pair if unmerged
                if (!merge_flag && trans_table_idx != util::ARP_CACHE_LEN) {
                    create_pair(in_ipv4->get_s_ip(), in_ipv4->get_s_mac());
                }

                uint16_t arp_op = ntohs(in_arp->get_op_code());
                if (arp_op == util::ARP_REQ) {
                    // reply with my ip & mac
                    reply(my_ip, my_mac, dev, in_eth, in_arp, in_ipv4);
                }
            }
        }
    }
}

bool arp_pro::update_pair(uint32_t ip, const uint8_t* mac) {
    for (int i = 0; i < trans_table_idx; ++i) {
        trans_entry& t = trans_table[i];

        if (t.pro_type == util::IPV4_TYPE && t.hw_type == util::ETH_HW_TYPE &&
            t.ip == ip) {
            memcpy(t.mac, mac, 6);

            return true;
        }
    }

    return false;
}

void arp_pro::create_pair(uint32_t ip, const uint8_t* mac) {
    trans_entry& t = trans_table[trans_table_idx];
    t.hw_type = util::ETH_HW_TYPE;
    t.pro_type = util::IPV4_TYPE;
    t.ip = ip;
    memcpy(t.mac, mac, 6);

    ++trans_table_idx;
}

void arp_pro::reply(
    uint32_t my_ip,
    uint8_t* my_mac,
    tap_device* dev,
    eth_hdr* in_eth,
    arp_hdr* in_arp,
    arp_ipv4* in_ipv4
) {
    // set dest addresses
    const uint8_t* dest_mac = in_ipv4->get_s_mac();
    uint32_t dest_ip = in_ipv4->get_s_ip();

    in_ipv4->set_d_ip(dest_ip);
    in_ipv4->set_d_mac(dest_mac);
    in_eth->set_dest(dest_mac);
    in_arp->set_op_code(htons(util::ARP_REP));

    // set source addresses
    in_ipv4->set_s_ip(htonl(my_ip));
    in_ipv4->set_s_mac(my_mac);
    in_eth->set_source(my_mac);

    cout << "Writing frame\n";

    cout << in_ipv4->to_string() << "\n";

    bool success = dev->write_eth(in_eth, util::FRAME_SIZE);

    if (success) {
        cout << "Successfully wrote frame\n";
    }
}
