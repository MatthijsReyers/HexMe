#include "settings.h"

settings loadSettings(const char* HOME)
{
    std::stringstream ss;
    ss << HOME << SETTINGSFILE;
    const char* url = ss.str().c_str();

    std::
}
