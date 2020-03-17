#pragma once

#include "./settings/settings.h"

#include "./gui/textbox/textbox.h"
#include "./gui/viewer/viewer.h"

#include "./utils/argparse/argparse.h"
#include "./utils/file/file.h"

#include <fstream>
#include <sstream>
#include <ncurses.h>
#include <locale.h>

#define byte char

class app
{
    private:
        const static int LINENUMBERSWIDTH = 8 + 2 + 2;
        const static int COLUMNWIDTH = (8 * 3 + 2) + (8 + 3);

        bool running;
        int columns;

        gui::textbox* cmdPromt;
        gui::viewer* hexView;

        utils::file& file;
        utils::arguments& args;

    public:
        app(utils::file& File, utils::arguments& Args);

        app& close();
        app& run();
        app& onResize();
        app& onMoveCursor(int n);
        app& executeCmd(const std::string& cmd);
};
