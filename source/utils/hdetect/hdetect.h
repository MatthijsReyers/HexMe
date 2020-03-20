#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

namespace utils
{
    std::string getFileHeaderType(std::fstream &file);
    unsigned long long getHeaderLength(const std::string &header);
}
