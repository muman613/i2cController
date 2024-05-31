#include <iostream>
#include <iomanip>
#include "i2cController.h"

#ifdef USE_PICO

#include "pico/stdlib.h"

#endif

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

auto test1() {
    i2cController i2c(0);

    if (i2c.open()) {
        std::cout << "OK" << std::endl;
        std::cout << i2c.info() << std::endl;

        std::cout << "Scanning bus..." << std::endl;

        auto devList = i2c.scan_bus();
        for (const auto &dev: devList) {
            std::cout << "Found device id : 0x" << std::hex << std::setw(2) << std::setfill('0') << (int) dev
                      << std::endl;
        }
        auto value = i2c.read_register(0x27, 0x00);

        std::cout << "Value 0x00 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;
        value = i2c.read_register(0x27, 0x01);
        std::cout << "Value 0x01 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;

        i2c.write_register(0x27, 0x01, 0x55);

        test_transceive_read_eeprom(i2c);
    }
}

auto test2() {
    i2cController i2c(1);
    if (i2c.open()) {
        std::cout << "OK" << std::endl;

        auto value = i2c.read_register(0x77, 0xd0);

        std::cout << "Device ID : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;
    } else {
        std::cerr << "Unable to open i2c device" << std::endl;
    }
}

#ifdef USE_PICO

auto test3() {
    i2cConfig cfg = {i2c0, 4, 5};
    i2cController i2c(0, &cfg);

    if (i2c.open()) {
        std::cout << "Open ok" << std::endl;

        auto value = i2c.read_register(0x27, 0x00);

        std::cout << "Value 0x00 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;
        value = i2c.read_register(0x27, 0x01);
        std::cout << "Value 0x01 : " << std::hex << std::setw(2) << std::setfill('0') << (int) value << std::endl;
    }
}

#endif


int main() {
    std::cout << "I2CController test suite" << std::endl;

#if defined(USE_I2CDRIVER)
    test1(); // test i2cdriver code running on host linux laptop
#elif defined(USE_LINUX)
    test2(); // test i2c device driver on the linux system
#elif defined(USE_PICO)
    stdio_init_all();

    while (true) {
        test3();
        sleep_ms(100);
    }

    // TODO
#endif

    return 0;
}
