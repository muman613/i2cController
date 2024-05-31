#include "i2cControllerLinux.h"
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>

extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}
namespace fs = std::filesystem;

i2cControllerLinux::i2cControllerLinux(uint8_t busNo, void *data) : i2cControllerBase(busNo) {

}

i2cControllerLinux::~i2cControllerLinux() {
    close();
}

bool i2cControllerLinux::open() {
#ifndef NDEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

    i2cDevicePath = "/dev/i2c-" + std::to_string(bus());

    if (fs::exists(i2cDevicePath)) {
        fd = ::open(i2cDevicePath.c_str(), O_RDWR);
        if (fd > 0) {
            connected = true;
        } else {
            std::cerr << "Unable to open i2c driver : " << strerror(errno) << std::endl;
        }
    } else {
        std::cerr << "Invalid i2c driver, no device on bus " << bus() << std::endl;
    }

    return connected;
}

void i2cControllerLinux::close() {
#ifndef NDEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

    if (fd > 0) {
        ::close(fd);
        fd = -1;
        connected = false;
    }
}

bool i2cControllerLinux::isOpen() {
    return ((bus() != (uint8_t) -1) && (fd != -1));
}

uint8_t i2cControllerLinux::read_register(uint8_t address, uint8_t reg) {
    if (set_slave_address(address)) {
        auto value32 = i2c_smbus_read_byte_data(fd, reg);
        if (value32 >= 0) {
            return (value32 & 0xff);
        } else {
            throw std::runtime_error(std::string("Unable to read : ") + strerror(value32));
        }
    } else {
        throw std::runtime_error("Unable to set slave address");
    }
}

void i2cControllerLinux::write_register(uint8_t address, uint8_t reg, uint8_t value) {
    if (connected) {
        if (set_slave_address(address)) {
            auto res = i2c_smbus_write_byte_data(fd, reg, value);
            if (res < 0) {
                throw std::runtime_error("Unable to write to register");
            }
        } else {
            throw std::runtime_error("Unable to set slave address");
        }
    } else {
        throw std::runtime_error("Not connected");
    }
}

bool i2cControllerLinux::transceive(uint8_t address, i2c_operator *operations, size_t len) {
    return false;
}

bool i2cControllerLinux::set_slave_address(uint8_t address) const {
    bool result = false;

    if (ioctl(fd, I2C_SLAVE, (int) address) == 0) {
        result = true;
    } else {
        std::cerr << "Unable to set address :" << strerror(errno) << std::endl;
    }

    return result;
}

std::vector<uint8_t> i2cControllerLinux::scan_bus() {
    return {};
}

std::string i2cControllerLinux::info() const {
    return {};
}
