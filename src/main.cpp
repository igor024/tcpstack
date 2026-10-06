#include "./tap/tap.hpp"
#include "./arp/arp_hdr.hpp"
#include "arp/arp_ipv4.hpp"
#include "ethernet/ethernet.hpp"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cstdint>
using namespace std;

constexpr int BUF_LEN = 100;
constexpr uint16_t IPV4_TYPE = 0x0800;
constexpr uint16_t ARP_TYPE = 0x0806;

int main() {
    cout << "Hi!";

    char dev_name[TAP_NAME_SIZE] = "tap0";
    tap_device* dev = new tap_device(dev_name);

    uint8_t buf[BUF_LEN];

    // attempt to ARP
    eth_hdr* hdr;

    while (true) {
        ssize_t res = dev->read_tap(buf, BUF_LEN, hdr);

        if (res <= 0) {
            cout << "read error\n";
            break;
        }

        // attempt to convert wire data to ethernet
        hdr = (eth_hdr*)buf;

        const uint16_t ethertype = ntohs(hdr->get_ethertype());

        if (ethertype == IPV4_TYPE) {
            cout << "IPv4 packet Incoming! \n";
           
            //do nothing
            
        }
        else if (ethertype == ARP_TYPE) {
            cout << "ARP Packet Incoming!\n";

            // attempt to convert payload into arp header
            
            arp_hdr* in_arp_hdr = (arp_hdr*) hdr->get_payload();

            // get ipv4 packet inside the header
            
            arp_ipv4* in_ipv4 = (arp_ipv4*) in_arp_hdr->get_payload();


            cout << in_ipv4->to_string();


        }

        /*

        const uint8_t* src = hdr->get_source();

        for (int i = 0; i < 6; ++i) {
            cout << *(src + i) << " ";
        }
*/
        cout << "\n";
    }
}
