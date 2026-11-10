#include "./arp/arp_hdr.hpp"
#include "./tap/tap.hpp"
#include "arp/arp_ipv4.hpp"
#include "ethernet/ethernet.hpp"
#include "util/util.hpp"
#include "./arp/arp_pro.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <sys/types.h>
#include <system_error>
using namespace std;

constexpr int BUF_LEN = 100;
constexpr uint16_t IPV4_TYPE = 0x0800;
constexpr uint16_t ARP_TYPE = 0x0806;
constexpr uint16_t ETH_HW_TYPE = 0x0001;
constexpr uint16_t ARP_REQ = 0x0001;
constexpr uint16_t ARP_REP = 0x0002;

uint16_t trans_table_idx = 0;

int main() {
    srand(time(NULL));

    // create dummy info

    uint8_t my_mac[6];
    util::rand_mac(my_mac);

    uint32_t my_ip = util::create_ip(10, 0, 0, 4);

    char dev_name[TAP_NAME_SIZE] = "tap0";
    tap_device* dev = new tap_device(dev_name);

    uint8_t buf[BUF_LEN];

    arp_pro arp = arp_pro();

    // attempt to ARP
    eth_hdr* in_eth;

    while (true) {
        ssize_t res = dev->read_tap(buf, BUF_LEN, in_eth);

        if (res <= 0) {
            cout << "read error\n";
            break;
        }

        // attempt to convert wire data to ethernet
        in_eth = (eth_hdr*)buf;

        const uint16_t ethertype = ntohs(in_eth->get_ethertype());

        if (ethertype == IPV4_TYPE) {
            cout << "IPv4 packet Incoming! \n";

            // do nothing

        } else if (ethertype == ARP_TYPE) {

            arp.handle_arp();
            
           /*

            const uint8_t* src = hdr->get_source();

            for (int i = 0; i < 6; ++i) {
                cout << *(src + i) << " ";
            }
            */
            cout << "\n";
        }
    }
}
