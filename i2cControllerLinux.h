#ifndef __I2CCONTROLLERLINUX_H__
#define __I2CCONTROLLERLINUX_H__

#include "i2cControllerBase.h"

class i2cControllerLinux : i2cControllerBase {
public:
    explicit i2cControllerLinux(uint8_t busNo, void *data = nullptr);

    ~i2cControllerLinux() override;

    bool open() override;

    void close() override;

    bool isOpen() override;

    uint8_t read_register(uint8_t address, uint8_t reg) override;

    void write_register(uint8_t address, uint8_t reg, uint8_t value) override;

    bool transceive(uint8_t address, i2c_operator operations[], size_t len) override;

    [[nodiscard]] std::vector<uint8_t> scan_bus() override;

    [[nodiscard]] std::string info() const override;

private:

    bool set_slave_address(uint8_t address) const;

    std::string i2cDevicePath;
    int fd = -1;
    bool connected = false;
};

#endif // __I2CCONTROLLERLINUX_H__
