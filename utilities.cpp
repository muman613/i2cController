
#include <string>
#include <map>
#include <fstream>
#include "utilities.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

