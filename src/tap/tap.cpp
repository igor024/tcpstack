#include "./tap.hpp"
#include <cstdint>
#include <cstring>

tap_device::tap_device(char* in_name) {
    strcpy(name, in_name);
    tap_alloc(name);
}

int tap_device::tap_alloc(char* name) {
    struct ifreq ifr{};

    int err = 0;
    fd = open("/dev/net/tun", O_RDWR);

    if (fd < 0) {
        cout << "Failed to open tap device. Exiting\n";
        exit(1);
    }

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

    if (*name) {
        strncpy(ifr.ifr_name, name, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        cout << "Ioctl failed: " << strerror(err);
        close(fd);
        return err;
    }

    strncpy(name, ifr.ifr_name, TAP_NAME_SIZE);

    return fd;
}

ssize_t tap_device::read_tap(uint8_t* buf, size_t buf_len, eth_hdr*& hdr) {
    hdr = nullptr; // if func does not complete will avoid dereferencing

    cout << "about to read from " << fd << "\n";

    ssize_t read_res = read(fd, buf, buf_len);

    cout << "read " << read_res << "\n";

    if (read_res < 0) {
        cerr << "TAP Read failed: " << strerror(errno) << "\n";
        return -1;
    }

    if (read_res == 0) {
        cout << "TAP device closed\n";
        return read_res;
    }

    if (static_cast<size_t>(read_res) < sizeof(eth_hdr)) {
        cerr << "Truncated frame. Read " << read_res
             << " bytes from TAP, expected " << sizeof(eth_hdr) << " bytes\n";
        // continue executing
    }

    hdr = reinterpret_cast<eth_hdr*>(buf);

    return read_res;
}

bool tap_device::write_eth(const eth_hdr* in_eth_hdr, size_t frame_size) {
    ssize_t bytes_written = write(fd, in_eth_hdr, frame_size);

    if (bytes_written < 0) {
        perror("Failed to write frame: ");
        return false;
    }

    if (static_cast<size_t>(bytes_written) < frame_size) {
        std::cerr << bytes_written << " bytes written out of " << frame_size
                  << "\n";
        return false;
    }

    return true;
}
