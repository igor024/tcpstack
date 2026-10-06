#include <cstdint>

class arp_hdr {
    public:
        arp_hdr(uint16_t hw_type, uint16_t pro_type, 
                uint8_t hw_size, uint8_t pro_size, uint16_t op_code);
        uint16_t get_hw_type();
        uint16_t get_pro_type();
        uint8_t get_hw_size();
        uint8_t get_pro_size();
        uint8_t* get_payload();

    private:
        uint16_t hw_type;  // typically 0x0001 for ethernet
        uint16_t pro_type; // typically 0x0800 for ethernet
        uint8_t hw_size;
        uint8_t pro_size;
        uint16_t op_code; // primarily ARP request (1) or ARP reply (2)

} __attribute__((packed));
