#include "util.hpp"
#include <cstddef>
#include <cstdint>
#include <format>
#include <string>
#include <time.h>

namespace util {

std::string format_mac(const uint8_t* mac) {
    return std::format(
        "{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}",
        mac[0],
        mac[1],
        mac[2],
        mac[3],
        mac[4],
        mac[5]
    );
}

std::string format_ip(const uint32_t ip) {
    std::string res = "";

    for (int i = 0; i < 4; ++i) {
        res += "{" + std::to_string((ip >> (24-8*i)) & 255) + "}";
        if (i < 3) {
            res += ".";
        }
    }

    return res;
}

uint32_t create_ip(const uint32_t a, const uint32_t b, const uint32_t c, const uint32_t d) {
    return (a << 24) + (b << 16) + (c << 8) + d;
}

void rand_mac(uint8_t* dest) {
    dest[0] = 0x02;

    for (int i = 1; i < 6; ++i) {
        dest[i] = rand() & 0xff;
    }
}

}
