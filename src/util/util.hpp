#include <cstdint>
#include <format>
#include <string>

namespace util {

std::string format_mac(const uint8_t* mac) {
    std::string res = "";
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

std::string format_ip(const uint8_t* ip) {
    std::string res = "";

    return std::format(
        "{}.{}.{}.{}",
        ip[0],
        ip[1],
        ip[2],
        ip[3]
    );
}

}
