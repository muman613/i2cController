#include "i2cControllerPico.h
#include "hardware/i2c.h"


i2cControllerPico::i2cControllerPico(uint8_t bus) : i2cControllerBase(bus), is_open(false) {}

bool i2cControllerPico::open() {
    // Implementation for opening the I2C connection
    is_open = true;
    std::cout << "i2cControllerPico: open called" << std::endl;
    return is_open;
}

void i2cControllerPico::close() {
    // Implementation for closing the I2C connection
    is_open = false;
    std::cout << "i2cControllerPico: close called" << std::endl;
}

bool i2cControllerPico::isOpen() {
    // Return the current state of the I2C connection
    return is_open;
}

uint8_t i2cControllerPico::read_register(uint8_t address, uint8_t reg) {
    // Implementation for reading a register
    std::cout << "i2cControllerPico: read_register called with address: "
              << static_cast<int>(address) << " reg: " << static_cast<int>(reg) << std::endl;
    // Return a dummy value for now
    return 0;
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

