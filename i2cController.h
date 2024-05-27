//
// Created by michael_uman on 5/20/24.
//

#ifndef I2CCONTROLLER_I2CCONTROLLER_H
#define I2CCONTROLLER_I2CCONTROLLER_H

#include <cstddef>
#include <string>
//
//#ifdef USE_I2CDRIVER
//extern "C" {
//#include "i2cdriver/i2cdriver.h"
//}
//#endif
//
//#ifdef USE_PICO
//
//#include "hardware/i2c.h"
//
//#endif
//

#ifdef USE_I2CDRIVER

#include "i2cControllerI2CDriver.h"

typedef i2cControllerI2CDriver i2cController;
#elif defined(USE_LINUX)

#include "i2cControllerLinux.h"

typedef i2cControllerLinux i2cController;
#elif defined( USE_PICO)

#include "i2cControllerPico.h"

typedef i2cControllerPico i2cController;
#endif
//
///**
// *
// */
//class i2cController {
//public:
//    explicit i2cController(uint8_t busNo, void *data = nullptr);
//
//    ~i2cController();
//
//    bool open();
//
//    bool close();
//
//    bool writeRegister(uint8_t addr, uint8_t reg, uint8_t value);
//
//    uint8_t readRegister(uint8_t addr, uint8_t reg);
//
//private:
//#ifdef USE_I2CDRIVER
//    I2CDriver i2c_driver = {};
//    std::string serialPort;
//#elif defined(USE_PICO)
//    uint8_t sda = 0;
//    uint8_t scl = 0;
//#else
//    int fd = -1;
//#endif
//
//    uint8_t busNumber = 0;
//};


#endif //I2CCONTROLLER_I2CCONTROLLER_H
