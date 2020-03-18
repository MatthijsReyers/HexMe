#include "converters.h"

namespace utils
{
    std::string byteToHexString(const byte b)
    {
        std::stringstream ss;
        int converted = (int)((unsigned char)b);
        if (converted < 0x0f) ss << "0";
        ss << std::hex << converted;
        return ss.str();
    }
}