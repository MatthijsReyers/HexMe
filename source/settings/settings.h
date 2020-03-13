#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

struct settings
{

};

settings loadSettings();
bool saveSettings(settings tosave);
