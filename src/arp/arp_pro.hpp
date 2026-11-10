#pragma once

#include "../ethernet/ethernet.hpp"
#include "../util/util.hpp"
#include "arp_ipv4.hpp"
#include "arp_hdr.hpp"
#include "../tap/tap.hpp"
#include <cstdint>

using namespace std;


struct trans_entry {
        uint16_t hw_type;
        uint8_t mac[6];
        uint16_t pro_type;
        uint32_t ip;
};

class arp_pro {
    public:
        void handle_arp(uint32_t my_ip, uint8_t* my_mac, tap_device* dev, eth_hdr* in_eth);

    private:
        /*
         * Returns merge_flag if the IP is found and updated in the table
         */
        bool update_pair(uint32_t ip, const uint8_t* mac);

        /*
         * adds an IP, MAC pair to the table in the case of an unrecognized IP
         */
        void create_pair(uint32_t ip, const uint8_t* mac);

        /*
         * transforms a broadcast message into an arp reply to the sender and
         * sends it */
        void reply(uint32_t my_ip, uint8_t* my_mac, tap_device* dev, eth_hdr* in_eth, arp_hdr* in_arp, arp_ipv4* in_ipv4);


        trans_entry trans_table[util::ARP_CACHE_LEN];
        uint16_t trans_table_idx;
};
