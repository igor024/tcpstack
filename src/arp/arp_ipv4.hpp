#pragma once

#include <cstdint>
#include <string>

#include <type_traits>

using namespace std;

class arp_ipv4 {
    public:
        arp_ipv4(
            uint8_t s_mac[6],
            uint8_t s_ip[4],
            uint8_t d_mac[6],
            uint8_t d_ip[4]
        );
        const uint8_t* get_s_mac() const;
        const uint8_t* get_s_ip() const;
        const uint8_t* get_d_mac() const;
        const uint8_t* get_d_ip() const;
        string to_string() const;

    private:
        string format_mac(const uint8_t* mac) const;
        string format_ip(const uint8_t* ip) const;
        uint8_t s_mac[6];
        uint8_t s_ip[4];
        uint8_t d_mac[6];
        uint8_t d_ip[4];
} __attribute__((packed));

static_assert(sizeof(arp_ipv4) == 20, "arp_ipv4 must be exactly 14 bytes");
static_assert(std::is_standard_layout<arp_ipv4>::value, "eth_hdr must be standard layout");
