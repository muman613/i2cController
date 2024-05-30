#ifndef __I2CCONTROLLERPICO_H__
#define __I2CCONTROLLERPICO_H__

#include "i2cControllerBase.h"

class i2cControllerPico : public i2cControllerBase {
public:
    explicit i2cControllerPico(uint8_t bus);

    bool open() override;

    void close() override;

    bool isOpen() override;

    uint8_t read_register(uint8_t address, uint8_t reg) override;

    void write_register(uint8_t address, uint8_t reg, uint8_t value) override;

    bool transceive(uint8_t address, i2c_operator operations[], size_t len) override;

private:
    bool is_open;
};


#endif // __I2CCONTROLLERPICO_H__
