//
// Created by michael_uman on 5/21/24.
//

#ifndef I2CCONTROLLER_I2CCONTROLLERBASE_H
#define I2CCONTROLLER_I2CCONTROLLERBASE_H

#include <cstddef>
#include <string>
#include <cstdint>
#include <vector>

enum class i2cOp {
    I2C_OPER_WRITE,
    I2C_OPER_READ,
};

struct i2c_operator {
    i2cOp op;
    uint8_t *data;
    uint8_t len;
};

#if defined(USE_PICO) || defined(USE_I2CDRIVER)
#define DEFAULT_BITRATE ((unsigned int)(100 * 1000))
#elif defined(USE_LINUX)
#define DEFAULT_BITRATE ((unsigned int)(400 * 1000))
#endif


class i2cControllerBase {
public:
    /**
     * i2cController constructor.
     *
     * @param bus i2c bus # (used on Linux and i2cDriver)
     * @param bitrate i2c bit rate in KBps.
     */
    explicit i2cControllerBase(uint8_t bus, unsigned int bitrate = DEFAULT_BITRATE);

    virtual ~i2cControllerBase();

    virtual bool open() = 0;

    virtual void close() = 0;

    virtual bool isOpen() = 0;

    virtual uint8_t read_register(uint8_t address, uint8_t reg) = 0;

    virtual void write_register(uint8_t address, uint8_t reg, uint8_t value) = 0;

    virtual bool transceive(uint8_t address, i2c_operator operations[], size_t len) = 0;

    [[nodiscard]] virtual std::vector<uint8_t> scan_bus() = 0;

    /**
     * Get driver information string.
     * @return
     */
    [[nodiscard]] virtual std::string info() const = 0;

    [[nodiscard]] inline uint8_t bus() const { return (uint8_t) busNo; }

    [[nodiscard]] inline unsigned int bitrate() const { return bitRate; }

private:
    int8_t busNo = -1;
    unsigned int bitRate = DEFAULT_BITRATE;
};

#endif //I2CCONTROLLER_I2CCONTROLLERBASE_H
