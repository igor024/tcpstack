#include <cstdint>
#include <format>
#include <string>

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

uint32_t create_ip(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d) {
    return (uint32_t) a + b * 256 + c * 256*256 + d * 256*256;
}

}
