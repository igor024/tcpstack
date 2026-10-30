#include "./arp_hdr.hpp"
#include <cstdint>

uint16_t arp_hdr::get_hw_type() {
    return hw_type;
}

uint16_t arp_hdr::get_pro_type() {
    return pro_type;
}

uint8_t arp_hdr::get_hw_size() {
    return hw_size;
}

uint8_t arp_hdr::get_pro_size() {
    return pro_size;
}

uint16_t arp_hdr::get_op_code() {
    return op_code;
}

void arp_hdr::set_op_code(uint16_t in_op_code) {
    op_code = in_op_code;
}

uint8_t* arp_hdr::get_payload() {
    return reinterpret_cast<uint8_t*>(this) + sizeof(*this);
}

