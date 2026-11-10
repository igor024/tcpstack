cmake_minimum_required(VERSION 3.23)
project(TCP_Server)

add_executable(tcp)

target_sources(tcp
    PRIVATE
        src/main.cpp
        src/util/util.cpp
        src/tap/tap.cpp
        src/arp/arp_hdr.cpp
        src/arp/arp_ipv4.cpp
        src/arp/arp_pro.cpp
        src/ethernet/ethernet.cpp
)
