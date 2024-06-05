#include "i2cControllerPico.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <iostream>
#include <sstream>


i2cControllerPico::i2cControllerPico(uint8_t bus, void *initData) : i2cControllerBase(bus), is_open(false),
                                                                    i2c_cfg(*(i2cConfig *) initData) {}

i2cControllerPico::~i2cControllerPico() {
    close();
}

bool i2cControllerPico::open() {
    // Implementation for opening the I2C connection
    i2c_init(i2c_cfg.i2c, 100 * 1000);

    gpio_set_function(i2c_cfg.i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(i2c_cfg.i2c_scl, GPIO_FUNC_I2C);

    gpio_pull_up(i2c_cfg.i2c_sda);
    gpio_pull_up(i2c_cfg.i2c_scl);

    is_open = true;

    return is_open;
}

void i2cControllerPico::close() {
    // Implementation for closing the I2C connection
    if (is_open) {
        is_open = false;
        std::cout << "i2cControllerPico: close called" << std::endl;

        i2c_deinit(i2c_cfg.i2c);

        gpio_set_function(i2c_cfg.i2c_sda, GPIO_FUNC_NULL);
        gpio_set_function(i2c_cfg.i2c_scl, GPIO_FUNC_NULL);

        gpio_disable_pulls(i2c_cfg.i2c_sda);
        gpio_disable_pulls(i2c_cfg.i2c_scl);
    }
}

bool i2cControllerPico::isOpen() {
    // Return the current state of the I2C connection
    return is_open;
}

uint8_t i2cControllerPico::read_register(uint8_t address, uint8_t reg) {
    if (i2c_write_blocking(i2c_cfg.i2c, address, &reg, 1, true) == PICO_ERROR_GENERIC) {
        std::cout << "ERROR: Unable to write." << std::endl;
    }
    uint8_t value = 0;
    if (i2c_read_blocking(i2c_cfg.i2c, address, &value, 1, false) == PICO_ERROR_GENERIC) {
        std::cout << "ERROR: Unable to read." << std::endl;
    }
    return value;
}

void i2cControllerPico::write_register(uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t packet[2] = {reg, value};
    if (i2c_write_blocking(i2c_cfg.i2c, address, packet, 2, false) == PICO_ERROR_GENERIC) {
        std::cout << "ERROR: Unable to write." << std::endl;
    }
}

bool i2cControllerPico::transceive(uint8_t address, i2c_operator operations[], size_t len) {
    // Implementation for transceive
    std::cout << "i2cControllerPico: transceive called with address: "
              << static_cast<int>(address) << " len: " << len << std::endl;
    // Return a dummy value for now
    return true;
}

// I2C reserves some addresses for special purposes. We exclude these from the scan. These are any addresses of the form 000 0xxx or 111 1xxx
static bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

std::vector<uint8_t> i2cControllerPico::scan_bus() {
    std::vector<uint8_t> devList;

    for (int addr = 0; addr < (1 << 7); ++addr) {
        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (!reserved_addr(addr)) {
            ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);
            if (ret >= 0) {
                devList.push_back(addr);
            }
        }
    }

    return devList;
}

std::string i2cControllerPico::info() const {
    std::ostringstream oss;

    oss << "i2cController     : i2cControllerPico" << std::endl;
    oss << "i2cControllerPico : i2c" << i2c_hw_index(i2c_cfg.i2c) << " sda=" << (int) i2c_cfg.i2c_sda << " scl="
        << (int) i2c_cfg.i2c_scl;

    return oss.str();
}
