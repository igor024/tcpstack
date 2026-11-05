#pragma once

#include <cstdint>
#include <string>

namespace util {
    std::string format_mac(const uint8_t* mac);
    std::string format_ip(const uint32_t ip);
    uint32_t create_ip(const uint32_t a, const uint32_t b, const uint32_t c, 
                       const uint32_t d);
    void rand_mac(uint8_t* dest);
}
