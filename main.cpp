#include <iostream>
#include <iomanip>
#include "i2cController.h"

void test_transceive_read_eeprom(i2cController &i2c) {
    uint8_t sendBuf[2] = {0x00, 0x00};
    uint8_t recBuf[4] = {};

    i2c_operator ops[] = {
            {i2cOp::I2C_OPER_WRITE, sendBuf, 2},
            {i2cOp::I2C_OPER_READ,  recBuf,  4},
    };

    i2c.transceive(0x50, ops, sizeof(ops) / sizeof(i2c_operator));

    for (const auto &c: recBuf) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int) c << std::endl;
    }
}

int main() {
    std::cout << "Hello, World! Goodbye..." << std::endl;

    i2cController i2c(0);

    if (i2c.open()) {
        std::cout << "OK" << std::endl;

        auto value = i2c.read_register(0x27, 0x00);

        std::cout << "Value 0x00 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;
        value = i2c.read_register(0x27, 0x01);
        std::cout << "Value 0x01 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;

        i2c.write_register(0x27, 0x01, 0x55);

        test_transceive_read_eeprom(i2c);
    }

//        i2cController i2c(0);
//    if (controllers[0].open()) {
//
//    }
    return 0;
}
