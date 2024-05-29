//
// Created by michael_uman on 5/21/24.
//

#ifndef I2CCONTROLLER_I2CCONTROLLERBASE_H
#define I2CCONTROLLER_I2CCONTROLLERBASE_H

#include <cstddef>
#include <string>
#include <cstdint>

enum class i2cOp {
    I2C_OPER_WRITE,
    I2C_OPER_READ,
};

struct i2c_operator {
    i2cOp op;
    uint8_t *data;
    uint8_t len;
};

class i2cControllerBase {
public:
    explicit i2cControllerBase(uint8_t bus);

    virtual ~i2cControllerBase();

    virtual bool open() = 0;

    virtual void close() = 0;

    virtual bool isOpen() = 0;

    virtual uint8_t read_register(uint8_t address, uint8_t reg) = 0;

    virtual void write_register(uint8_t address, uint8_t reg, uint8_t value) = 0;

    virtual bool transceive(uint8_t address, i2c_operator operations[], size_t len) = 0;


    inline uint8_t bus() const { return (uint8_t) busNo; }


private:
    int8_t busNo = -1;
};

#endif //I2CCONTROLLER_I2CCONTROLLERBASE_H
