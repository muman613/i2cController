#ifndef __I2CCONTROLLERPICO_H__
#define __I2CCONTROLLERPICO_H__

#include "i2cControllerBase.h"
#include "hardware/i2c.h"

typedef struct {
    i2c_inst_t *i2c = i2c0;
    uint8_t i2c_sda = 4;
    uint8_t i2c_scl = 5;
} i2cConfig;

class i2cControllerPico : public i2cControllerBase {
public:
    explicit i2cControllerPico(uint8_t bus, void *data = nullptr);

    virtual ~i2cControllerPico();

    bool open() override;

    void close() override;

    bool isOpen() override;

    uint8_t read_register(uint8_t address, uint8_t reg) override;

    void write_register(uint8_t address, uint8_t reg, uint8_t value) override;

    bool transceive(uint8_t address, i2c_operator operations[], size_t len) override;

    std::vector<uint8_t> scan_bus() override;

    [[nodiscard]] std::string info() const override;

private:
    bool is_open;
    i2cConfig i2c_cfg = {};
};


#endif // __I2CCONTROLLERPICO_H__
