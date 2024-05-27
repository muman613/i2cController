#include "i2cControllerI2CDriver.h"
#include <iostream>
#include "utilities.h"

i2cControllerI2CDriver::i2cControllerI2CDriver(uint8_t bus, void *data) : i2cControllerBase(bus) {

}

i2cControllerI2CDriver::~i2cControllerI2CDriver() {
    close();
}

bool i2cControllerI2CDriver::open() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    serialPort = getI2cSerialPortFromBus();
    i2c_connect(&i2c_driver, serialPort.c_str());
    if (i2c_driver.connected != 1) {
        std::cerr << "Unable to open i2cdriver..." << std::endl;
        return false;
    }

    displayControllerInfo();

    return true;
}


void i2cControllerI2CDriver::close() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    i2c_disconnect(&i2c_driver);
}

bool i2cControllerI2CDriver::isOpen() {
    return (i2c_driver.connected == 1);
}

uint8_t i2cControllerI2CDriver::read_register(uint8_t address, uint8_t reg) {
    uint8_t value = 0;

    i2c_start(&i2c_driver, address, OPER_WRITE);
    i2c_write(&i2c_driver, &reg, 1);
    i2c_start(&i2c_driver, address, OPER_READ);
    i2c_read(&i2c_driver, &value, 1);
    i2c_stop(&i2c_driver);

    return value;
}

void i2cControllerI2CDriver::write_register(uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = value;

    i2c_start(&i2c_driver, address, OPER_WRITE);
    i2c_write(&i2c_driver, buffer, 2);
    i2c_stop(&i2c_driver);
}

bool i2cControllerI2CDriver::transceive(uint8_t address, i2c_operator operations[], size_t len) {
    for (int i = 0; i < len; i++) {
        const auto op = operations[i];

        if (op.op == i2cOp::I2C_OPER_WRITE) {
            i2c_start(&i2c_driver, address, OPER_WRITE);
            i2c_write(&i2c_driver, op.data, op.len);
        } else if (op.op == i2cOp::I2C_OPER_READ) {
            i2c_start(&i2c_driver, address, OPER_READ);
            i2c_read(&i2c_driver, op.data, op.len);
        }
    }

    i2c_stop(&i2c_driver);

    return true;
}


static inline std::string getHome() {
    return getenv("HOME");
}

std::string i2cControllerI2CDriver::getI2cSerialPortFromBus() const {
    std::string fullPath = getHome() + "/.i2cmap";
    auto busMap = readIniFile(fullPath);
    std::string key = "bus" + std::to_string(bus());
    if (busMap.find(key) != busMap.end()) {
        return busMap[key]["serial"];
    }
    return {};
}

void i2cControllerI2CDriver::displayControllerInfo() {
    std::cout << "i2cDriver : " << i2c_driver.model << " - " << i2c_driver.serial << std::endl;
    std::cout << "i2cDriver : " << i2c_driver.voltage_v << "V " << i2c_driver.current_ma << "mA" << std::endl;
}

