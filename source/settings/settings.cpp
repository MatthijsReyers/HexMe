#include "settings.h"

settings loadSettings()
{
    const static char* SETTINGSFILE = "settings.ini";
    const static char* SETTINGSDIR = "/.local/share/hexme/";
    const char* HOMEDIR = getenv("HOME");

    std::stringstream ss;
    ss << HOMEDIR << SETTINGSDIR;
    std::string directory = ss.str();
    ss << SETTINGSFILE;
    std::string url = ss.str();

    std::fstream file(url.c_str(), std::fstream::out);
    if (!file) {
        mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        file.open(url.c_str(), std::fstream::out);
        if (!file)
            throw CouldNotLoadSettingsException("Could not load settings file and failed to create new file settings file.");
    }

    file.close();
}

bool saveSettings(const settings tosave)
{
    return true;
}
