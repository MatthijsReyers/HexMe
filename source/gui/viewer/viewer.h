#pragma once

#include "./../../utils/file/file.h"
#include "ncurses.h"
#include <iostream>

namespace gui
{
    class viewer
    {
    private:
        const static int LINENUMBERSWIDTH = 8 + 2 + 2;
        const static int COLUMNWIDTH = (8 * 3 + 2) + (8 + 3);

        WINDOW* window;
        utils::file file;

        int width, height;
        int columns, rows;
        
        int rowIndex = 0;

        void drawRow(long r);
        void drawBorders();

        unsigned long int getRowFromIndex(const unsigned long long index);
        unsigned long int getYFromIndex(const unsigned long long index);

    public:
        viewer(utils::file File);
        ~viewer();

        viewer& onRefresh();
        viewer& onResize();
    };
}
