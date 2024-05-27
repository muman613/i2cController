#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <string>
#include <map>
#include <fstream>

std::map<std::string, std::map<std::string, std::string>> readIniFile(const std::string &filename);

#endif // __UTILITIES_H__
