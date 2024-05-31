#include "i2cControllerPico.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <iostream>


i2cControllerPico::i2cControllerPico(uint8_t bus, void *initData) : i2cControllerBase(bus), is_open(false),
                                                                    i2c_cfg(*(i2cConfig *) initData) {}

i2cControllerPico::~i2cControllerPico() {
    close();
}

bool i2cControllerPico::open() {
    // Implementation for opening the I2C connection
    is_open = true;
    std::cout << "i2cControllerPico: open called" << std::endl;
    i2c_init(i2c_cfg.i2c, 100 * 1000);

    std::cout << "SDA : " << (int) i2c_cfg.i2c_sda << " SCL : " << (int) i2c_cfg.i2c_scl << std::endl;

    gpio_set_function(i2c_cfg.i2c_sda, GPIO_FUNC_I2C);
    gpio_set_function(i2c_cfg.i2c_scl, GPIO_FUNC_I2C);

    gpio_pull_up(i2c_cfg.i2c_sda);
    gpio_pull_up(i2c_cfg.i2c_scl);

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
    // Implementation for reading a register
//    std::cout << "i2cControllerPico: read_register called with address: "
//              << static_cast<int>(address) << " reg: " << static_cast<int>(reg) << std::endl;

    if (i2c_write_blocking(i2c_cfg.i2c, address, &reg, 1, true) == PICO_ERROR_GENERIC) {
        std::cout << "ERROR: Unable to write." << std::endl;
    }
    uint8_t value = 0;
    if (i2c_read_blocking(i2c_cfg.i2c, address, &value, 1, true) == PICO_ERROR_GENERIC) {
        std::cout << "ERROR: Unable to read." << std::endl;
    }

    return value;
}

void i2cControllerPico::write_register(uint8_t address, uint8_t reg, uint8_t value) {
    // Implementation for writing to a register
    std::cout << "i2cControllerPico: write_register called with address: "
              << static_cast<int>(address) << " reg: " << static_cast<int>(reg)
              << " value: " << static_cast<int>(value) << std::endl;
}

bool i2cControllerPico::transceive(uint8_t address, i2c_operator operations[], size_t len) {
    // Implementation for transceive
    std::cout << "i2cControllerPico: transceive called with address: "
              << static_cast<int>(address) << " len: " << len << std::endl;
    // Return a dummy value for now
    return true;
}

std::vector<uint8_t> i2cControllerPico::scan_bus() {
    return {};
}

std::string i2cControllerPico::info() const {
    return {};
}
