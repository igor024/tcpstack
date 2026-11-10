#pragma once

#include "../arp/arp_hdr.hpp"
#include "../arp/arp_ipv4.hpp"
#include "../ethernet/ethernet.hpp"
#include <cstdint>
#include <string>

namespace util {
    constexpr size_t frame_size =
        sizeof(eth_hdr) + sizeof(arp_hdr) + sizeof(arp_ipv4);

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
