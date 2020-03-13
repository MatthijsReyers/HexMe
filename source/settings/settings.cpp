#include "settings.h"

settings loadSettings()
{
    const char* SETTINGSFILE = "settings.ini";
    const char* SETTINGSDIR = "/.local/share/hexme/";
    const char* HOMEDIR = getenv("HOME");

    std::stringstream ss;
    ss << HOMEDIR << SETTINGSDIR;
    std::string directory = ss.str();
    ss << SETTINGSFILE;
    std::string url = ss.str();

    std::fstream file(url.c_str(), std::fstream::out);
    if (!file) {
        mkdir();
        file.open(url.c_str(), std::fstream::out);
    }

    ss.clear();
    std::stringstream  newSS;
    newSS << "mkdir " << HOMEDIR << "/.local/share/hexme/";
    system(newSS.str().c_str());

    // printf(ss.str().c_str());


    if (!file) printf("\nIts not okay!\n");
    else printf("\nIts okay!\n");
}

bool saveSettings(settings tosave)
{
    return true;
}
