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

i2cControllerLinux::i2cControllerLinux(uint8_t busNo, unsigned int bitrate, void *data) : i2cControllerBase(busNo,
                                                                                                            bitrate) {

}

i2cControllerLinux::~i2cControllerLinux() {
    close();
}

bool i2cControllerLinux::open() {
    if (bitrate() != DEFAULT_BITRATE) {
#ifndef NDEBUG
        std::cerr << "Unable to change bitrate to " << bitrate() << std::endl;
#endif
        return false;
    }

    i2cDevicePath = "/dev/i2c-" + std::to_string(bus());

    if (fs::exists(i2cDevicePath)) {
        fd = ::open(i2cDevicePath.c_str(), O_RDWR);
        if (fd > 0) {
            connected = true;
        } else {
#ifndef NDEBUG
            std::cerr << "Unable to open i2c driver : " << strerror(errno) << std::endl;
#endif
        }
    } else {
#ifndef NDEBUG
        std::cerr << "Invalid i2c driver, no device on bus " << bus() << std::endl;
#endif
    }


    return connected;
}

void i2cControllerLinux::close() {
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
    // Translate input operations to i2c_msg structures
    std::vector<i2c_msg> msgs;
    for (size_t i = 0; i < len; ++i) {
        i2c_msg msg{};
        std::memset(&msg, 0, sizeof(msg));
        msg.addr = address;
        msg.flags = (operations[i].op == i2cOp::I2C_OPER_READ) ? I2C_M_RD : 0;
        msg.len = operations[i].len;
        msg.buf = operations[i].data;
        msgs.push_back(msg);
    }

    // Create i2c_rdwr_ioctl_data structure
    i2c_rdwr_ioctl_data ioctl_data{};

    ioctl_data.msgs = msgs.data();
    ioctl_data.nmsgs = msgs.size();

    if (ioctl(fd, I2C_RDWR, &ioctl_data) < 0) {
        return false;
    }

    return true;
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
    std::vector<uint8_t> devList;

    int first = 0x03, last = 0x77;
    int res;

    for (int devId = first; devId <= last; devId++) {
        // Do not include devices whose I2C interface is controlled by device driver (UU)
        if (ioctl(fd, I2C_SLAVE, devId) < 0) {
            continue;
        }
        res = i2c_smbus_read_byte(fd);
        if (res >= 0) {
            devList.push_back(devId);
        }
    }
    return devList;
}

std::string i2cControllerLinux::info() const {
    std::ostringstream oss;

    oss << "i2cController       : i2cControllerLinux" << std::endl;
    oss << "i2cControllerLinux  : bus " << (int) bus();

    return oss.str();
}
