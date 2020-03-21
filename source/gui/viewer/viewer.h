#pragma once

#include "./../../utils/file/file.h"
#include "./../../utils/hdetect/hdetect.h"
#include "./../../utils/converters/converters.h"
#include "ncurses.h"
#include <iostream>
#include <sstream>
#include <bitset>

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

        unsigned long long topRow = 0;
        unsigned long long headerLength = 0;

        void drawRow(unsigned long long r);
        void drawBorders();

        int getByteColor(byte b, unsigned long long index, unsigned long long cursor);

        unsigned long long getYofCursor(const unsigned long long cursor);
        unsigned long long getXofCursor(const unsigned long long cursor);

    public:
        viewer(utils::file File);
        ~viewer();
        
        viewer& onRefresh();
        viewer& onResize();
        viewer& onMoveCursor(int direction);

        int getColumnCount();
    };
}
