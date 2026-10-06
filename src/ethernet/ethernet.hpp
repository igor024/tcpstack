#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

class eth_hdr {
    public:
        void set_dest(const uint8_t* mac);
        const uint8_t* get_dest() const;

        void set_source(const uint8_t* mac);
        const uint8_t* get_source() const;

        void set_ethertype(uint16_t in_ethertype);
        const uint16_t get_ethertype() const;

        const uint8_t* get_payload() const; // for read-only
        uint8_t* get_payload();


    private:
        uint8_t d_mac[6]; // each block is 0-255 in hexadecimal
        uint8_t s_mac[6];
        uint16_t ethertype;
} __attribute__((packed)); // compiler will not pad the end of the struct

static_assert(sizeof(eth_hdr) == 14, "eth_hdr must be exactly 14 bytes");
static_assert(std::is_standard_layout<eth_hdr>::value, "eth_hdr must be standard layout");
