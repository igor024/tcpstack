#include <cstdint>
#include <cstring>
#include <format>
#include <string>


#include "./arp_ipv4.hpp"
#include "../util/util.hpp"

using namespace std;

constexpr uint8_t MAC_LEN = 6;

constexpr uint8_t IP_LEN = 4;

const uint8_t* arp_ipv4::get_s_mac() const {
    return s_mac;
}

void arp_ipv4::set_s_mac(const uint8_t* in_s_mac) {
    memcpy(s_mac, in_s_mac, 6);
}


const uint32_t arp_ipv4::get_s_ip() const {
    return s_ip;
}

void arp_ipv4::set_s_ip(const uint16_t in_s_ip) {
    s_ip = in_s_ip;
}

const uint8_t* arp_ipv4::get_d_mac() const {
    return d_mac;
}

void arp_ipv4::set_d_mac(const uint8_t* in_d_mac) {
    memcpy(d_mac, in_d_mac, 6);
}

const uint32_t arp_ipv4::get_d_ip() const {
    return d_ip;
}

void arp_ipv4::set_d_ip(const uint16_t in_d_ip) {
    s_ip = in_d_ip;
}


string arp_ipv4::to_string() const {
    string mac = util::format_mac(s_mac);
    return "ARP: IPV4 ADDRESS FIELDS\n"
           "source mac: " +
           util::format_mac(s_mac) + " destination mac: " + util::format_mac(d_mac) + "\n" +
           "source ip: " + util::format_ip(s_ip) + " "
           "destination ip: " + util::format_ip(d_ip) + "\n";
}
