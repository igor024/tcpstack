#pragma once

#include "../arp/arp_hdr.hpp"
#include "../arp/arp_ipv4.hpp"
#include "../ethernet/ethernet.hpp"
#include <cstdint>
#include <string>

namespace util {
    constexpr size_t FRAME_SIZE =
        sizeof(eth_hdr) + sizeof(arp_hdr) + sizeof(arp_ipv4);
    constexpr int BUF_LEN = 100;
    constexpr uint16_t IPV4_TYPE = 0x0800;
    constexpr uint16_t ARP_TYPE = 0x0806;
    constexpr uint16_t ETH_HW_TYPE = 0x0001;
    constexpr uint16_t ARP_REQ = 0x0001;
    constexpr uint16_t ARP_REP = 0x0002;
    constexpr uint16_t ARP_CACHE_LEN = 0x0010;
    std::string format_mac(const uint8_t* mac);
    std::string format_ip(const uint32_t ip);
    uint32_t create_ip(
        const uint32_t a,
        const uint32_t b,
        const uint32_t c,
        const uint32_t d
    );
    void rand_mac(uint8_t* dest);
} // namespace util
