#pragma once

#include "./../file/file.h"

#include <string>
#include <vector>
#include <map>

#define byte char

namespace utils
{
    std::string getFileHeaderType(utils::file& f);
    unsigned long long getHeaderLength(const std::string& header);
}
