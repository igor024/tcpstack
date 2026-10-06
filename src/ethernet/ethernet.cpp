#include "./ethernet.hpp"
#include <cstdint>

using namespace std;

void eth_hdr::set_dest(const uint8_t* mac) {
    memcpy(d_mac, mac, 6);
}

const uint8_t* eth_hdr::get_dest() const {
    return d_mac;
}

void eth_hdr::set_source(const uint8_t* mac) {
    memcpy(s_mac, mac, 6);
}

const uint8_t* eth_hdr::get_source() const {
    return s_mac;
}

void eth_hdr::set_ethertype(uint16_t in_ethertype) {
    ethertype = in_ethertype;
}

const uint16_t eth_hdr::get_ethertype() const {
    return ethertype;
}

const uint8_t* eth_hdr::get_payload() const {
    return reinterpret_cast<const uint8_t*>(this)
        + sizeof(*this);
}

uint8_t* eth_hdr::get_payload() {
    return reinterpret_cast<uint8_t*>(this)
        + sizeof(*this);
}

