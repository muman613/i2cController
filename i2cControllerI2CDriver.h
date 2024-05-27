#ifndef __I2CCONTROLLERI2CDRIVER_H__
#define __I2CCONTROLLERI2CDRIVER_H__

#include <string>

extern "C" {
#include "i2cdriver/i2cdriver.h"
}

#include "i2cControllerBase.h"

class i2cControllerI2CDriver : public i2cControllerBase {
public:
    explicit i2cControllerI2CDriver(uint8_t busNo, void *data = nullptr);

    virtual ~i2cControllerI2CDriver();

    bool open() override;

    void close() override;

    bool isOpen() override;

    uint8_t read_register(uint8_t address, uint8_t reg) override;

    void write_register(uint8_t address, uint8_t reg, uint8_t value) override;

    bool transceive(uint8_t address, i2c_operator operations[], size_t len) override;

private:
    [[nodiscard]] std::string getI2cSerialPortFromBus() const;

    I2CDriver i2c_driver = {};
    std::string serialPort;

    void displayControllerInfo();
};

#define OPER_READ 1
#define OPER_WRITE 0

#endif // __I2CCONTROLLERI2CDRIVER_H__