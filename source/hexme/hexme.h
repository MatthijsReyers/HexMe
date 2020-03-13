#pragma once

#include "./../settings/settings.h"
#include "./../argparse/argparse.h"

#include <fstream>
#include <sstream>
#include <ncurses.h>

class hexme
{
    private:
        int width, height;
        int selectedX, selectedY;
        std::fstream file;
        arguments args;
        settings sets;

    public:
        hexme() {};
        hexme(std::fstream &file, arguments args);

        void close();
        void run();
        void onResize();
};