#pragma once

#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <linux/if_ether.h>
#include <linux/if_tun.h>
#include <memory>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#include "../ethernet/ethernet.hpp"

#define TAP_NAME_SIZE 15

using namespace std;

class tap_device {
    public:
        tap_device(char* in_name);
        ssize_t read_tap(uint8_t* buf, size_t buf_len, eth_hdr*& hdr);

    private:
        int tap_alloc(char* name);

        char name[TAP_NAME_SIZE];
        int fd;
};
