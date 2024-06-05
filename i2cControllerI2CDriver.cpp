#include "i2cControllerI2CDriver.h"
#include <iostream>
#include <sstream>
#include "utilities.h"
#include <cstdio>
#include <cstring>
#include <unistd.h>

i2cControllerI2CDriver::i2cControllerI2CDriver(uint8_t bus, unsigned int bitrate, void *data)
        : i2cControllerBase(bus, bitrate) {
}

i2cControllerI2CDriver::~i2cControllerI2CDriver() {
    close();
}

bool i2cControllerI2CDriver::open() {
    serialPort = getI2cSerialPortFromBus();
    ::i2c_connect(&i2c_driver, serialPort.c_str());
    if (i2c_driver.connected != 1) {
        std::cerr << "Unable to open i2cdriver..." << std::endl;
        return false;
    }

    // Set the baudrate for the i2cDriver (specified in kHz)
    if (!::i2c_setbaud(&i2c_driver, bitrate() / 1000)) {
        std::cerr << "Unable to set baudrate" << std::endl;
        return false;
    }

    return true;
}


void i2cControllerI2CDriver::close() {
    ::i2c_disconnect(&i2c_driver);
}

bool i2cControllerI2CDriver::isOpen() {
    return (i2c_driver.connected == 1);
}

uint8_t i2cControllerI2CDriver::read_register(uint8_t address, uint8_t reg) {
    uint8_t value = 0;

    ::i2c_start(&i2c_driver, address, OPER_WRITE);
    ::i2c_write(&i2c_driver, &reg, 1);
    ::i2c_start(&i2c_driver, address, OPER_READ);
    ::i2c_read(&i2c_driver, &value, 1);
    ::i2c_stop(&i2c_driver);

    return value;
}

void i2cControllerI2CDriver::write_register(uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = value;

    ::i2c_start(&i2c_driver, address, OPER_WRITE);
    ::i2c_write(&i2c_driver, buffer, 2);
    ::i2c_stop(&i2c_driver);
}

bool i2cControllerI2CDriver::transceive(uint8_t address, i2c_operator operations[], size_t len) {
    for (int i = 0; i < len; i++) {
        const auto op = operations[i];

        if (op.op == i2cOp::I2C_OPER_WRITE) {
            ::i2c_start(&i2c_driver, address, OPER_WRITE);
            ::i2c_write(&i2c_driver, op.data, op.len);
        } else if (op.op == i2cOp::I2C_OPER_READ) {
            ::i2c_start(&i2c_driver, address, OPER_READ);
            ::i2c_read(&i2c_driver, op.data, op.len);
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


// Helper function to split the string by space and parse hex values.
static std::vector<uint8_t> parseLine(const std::string &line) {
    std::vector<uint8_t> addresses;
    std::istringstream iss(line);
    std::string word;

    while (iss >> word) {
        if (word != "--") {
            // Convert string hex value to int and then cast to uint8_t
            addresses.push_back(static_cast<uint8_t>(std::stoi(word, nullptr, 16)));
        }
    }

    return addresses;
}

[[nodiscard]] std::vector<uint8_t> parse_scan_string(const std::string &bus_scan) {
    std::vector<uint8_t> devices;
    std::istringstream iss(bus_scan);
    std::string line;

    // Process each line of the input string
    while (std::getline(iss, line)) {
        std::vector<uint8_t> lineAddresses = parseLine(line);
        // append all addresses from the line to the devices vector
        devices.insert(devices.end(), lineAddresses.begin(), lineAddresses.end());
    }

    return devices;
}

[[nodiscard]] std::vector<uint8_t> i2cControllerI2CDriver::scan_bus() {
    char *args[] = {
            (char *) "d",
    };
    // Create a string stream to capture the output
    std::ostringstream stringStream;

    // Save the original stdout file descriptor
    int originalStdoutFd = ::dup(STDOUT_FILENO);

    // Create a temporary file
    FILE *tempFile = ::tmpfile();

    // Just to get a valid FILE*
    if (::freopen("/dev/null", "a", stdout) == nullptr) {
        ::fclose(tempFile);
        return {};
    }

    int tempFileFd = fileno(tempFile);
    dup2(tempFileFd, STDOUT_FILENO);

    ::i2c_commands(&i2c_driver, 1, args);

    // Flush stdout to ensure all output is written to the temporary file
    ::fflush(stdout);

    // Restore the original stdout
    ::dup2(originalStdoutFd, STDOUT_FILENO);
    ::close(originalStdoutFd);

    // Read the output from the temporary file into the string stream
    ::fseek(tempFile, 0, SEEK_SET);
    char buffer[1024];
    while (::fgets(buffer, sizeof(buffer), tempFile) != nullptr) {
        stringStream << buffer;
    }
    // Close the temporary file
    ::fclose(tempFile);

    // Retrieve the captured output from the string stream
    std::string capturedOutput = stringStream.str();
    return parse_scan_string(capturedOutput);
}

std::string i2cControllerI2CDriver::info() const {
    std::ostringstream oss;

    oss << "i2cController : i2cDriver" << std::endl;
    oss << "i2cDriver     : " << i2c_driver.model << " - " << i2c_driver.serial << std::endl;
    oss << "i2cDriver     : " << i2c_driver.voltage_v << "V " << i2c_driver.current_ma << "mA" << std::endl;
    oss << "i2cDriver     : " << i2c_driver.speed << "KBps SDA = " << i2c_driver.sda << " SCL = " << i2c_driver.scl;

    return oss.str();
}

