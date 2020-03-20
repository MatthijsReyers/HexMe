#pragma once

#include "./../file/file.h"

#include <string>
#include <vector>
#include <map>

namespace utils
{
    std::string getFileHeaderType(utils::file& f);
    unsigned long long getHeaderLength(const std::string& header);
}
