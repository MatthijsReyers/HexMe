#pragma once

#include "./settings/settings.h"

#include "./gui/commandline/commandline.h"
// #include "./gui/main/main.h"

#include "./utils/argparse/argparse.h"
#include "./utils/file/file.h"

#include <fstream>
#include <sstream>
#include <ncurses.h>

class app
{
    private:
        const static int LINENUMBERSWIDTH = 8 + 2 + 2;
        const static int COLUMNWIDTH = (8 * 3 + 2) + (8 + 3);

        int width, height;
        int rows, columns;

        bool running;
        bool focusCmdPromt = false;

        gui::commandline cmdWindow;
        // gui::main mainWindow;

        utils::file& file;
        utils::arguments& args;
        settings sets;

    public:
        app(utils::file& File, utils::arguments& Args);

        app& close();
        app& run();
        app& onResize();
        app& onMoveCursor(int n);
        app& executeCmd(const std::string& cmd);
};
