//
// Created by michael_uman on 5/20/24.
//

#ifndef I2CCONTROLLER_I2CCONTROLLER_H
#define I2CCONTROLLER_I2CCONTROLLER_H

#include <cstddef>
#include <string>

#ifdef USE_I2CDRIVER

#include "i2cControllerI2CDriver.h"

typedef i2cControllerI2CDriver i2cController;
#elif defined(USE_LINUX)

#include "i2cControllerLinux.h"

typedef i2cControllerLinux i2cController;
#elif defined(USE_PICO)

#include "i2cControllerPico.h"

typedef i2cControllerPico i2cController;
#endif

#endif //I2CCONTROLLER_I2CCONTROLLER_H

