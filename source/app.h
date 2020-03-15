#pragma once

#include "./settings/settings.h"
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

        utils::file& file;
        utils::arguments& args;
        settings sets;

    public:
        app(utils::file& File, utils::arguments& Args);

        void close();
        void run();
        void onResize();
};
