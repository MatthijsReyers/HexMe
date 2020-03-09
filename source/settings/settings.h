#include <fstream>

const char* SETTINGSFILE = ".local/share/hexme/settings.ini";
const char* HOMEDIR = getenv("HOME");

struct settings
{

};

settings loadSettings(const char* HOME);