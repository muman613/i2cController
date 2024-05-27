//
// Created by michael_uman on 5/20/24.
//

#include "i2cController.h"
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

#ifdef USE_I2CDRIVER

std::string getHome() {
    return getenv("HOME");
}

// Function to trim whitespace from both ends of a string
inline std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

std::map<std::string, std::map<std::string, std::string>> readIniFile(const std::string &filename) {
    std::map<std::string, std::map<std::string, std::string>> iniMap;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return iniMap;
    }

    std::string line;
    std::string currentSection;
    while (std::getline(file, line)) {
        // Remove comments and trim the line
        line = line.substr(0, line.find(';'));
        line = trim(line);

        if (line.empty()) {
            // Skip empty lines
            continue;
        }

        if (line[0] == '[' && line.back() == ']') {
            // This is a section header
            currentSection = trim(line.substr(1, line.size() - 2));
        } else {
            // This is a key-value pair
            std::istringstream lineStream(line);
            std::string key, value;
            if (std::getline(lineStream, key, '=') && std::getline(lineStream, value)) {
                key = trim(key);
                value = trim(value);
                iniMap[currentSection][key] = value;
            }
        }
    }

    file.close();
    return iniMap;
}

std::string getI2cSerialPortFromBus(uint8_t bus) {
    std::string fullPath = getHome() + "/.i2cmap";
    auto busMap = readIniFile(fullPath);
    std::string key = "bus" + std::to_string(bus);
    if (busMap.find(key) != busMap.end()) {
        return busMap[key]["serial"];
    }
    return {};
}

#endif


i2cController::i2cController(uint8_t bus, void *data) : busNumber(bus) {
    cout << __PRETTY_FUNCTION__ << " I2C Bus # " << (int) busNumber << endl;


    cout << "OK" << endl;
}


i2cController::~i2cController() {
    cout << __PRETTY_FUNCTION__ << endl;
}


bool i2cController::open() {
#ifdef USE_I2CDRIVER
    serialPort = getI2cSerialPortFromBus(busNumber);
    i2c_connect(&i2c_driver, serialPort.c_str());
    if (i2c_driver.connected != 1) {
        cerr << "Unable to open i2cdriver..." << endl;
        return false;
    }
    char *args[] = {
            (char *) "d",
            0,
    };
    i2c_commands(&i2c_driver, 1, args);

    return true;
#endif
}