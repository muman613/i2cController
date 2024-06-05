//
// Created by michael_uman on 5/21/24.
//

#include "i2cControllerBase.h"

i2cControllerBase::i2cControllerBase(uint8_t bus, unsigned int bitrate) : busNo((int8_t) bus), bitRate(bitrate) {

}

i2cControllerBase::~i2cControllerBase() = default;
