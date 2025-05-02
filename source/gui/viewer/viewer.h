#pragma once

#include "./../../utils/file/file.h"
#include "./../../utils/hdetect/hdetect.h"
#include "./../../utils/converters/converters.h"
#include "ncurses.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include <optional>

namespace gui
{
    class viewer
    {
    private:
        const static int LINE_NUMBERS_WIDTH = 8 + 2 + 2;
        const static int COLUMN_WIDTH = (8 * 3 + 2) + (8 + 3);

        WINDOW* window;
        utils::file file;

        u_int32_t width, height;
        u_int32_t columns, rows; 

        u_int64_t topRow = 0;
        u_int64_t headerLength = 0;

        void drawRow(u_int64_t r);
        void drawBorders();

        int getByteColor(byte b, u_int64_t index, u_int64_t cursor);

        u_int64_t getYofCursor(const u_int64_t cursor);
        u_int64_t getXofCursor(const u_int64_t cursor);

    public:
        /**
         * Maximum amount of columns the hex view is allowed to display, note that the UI will not
         * reflect changes to this value until onRefresh() is called.
         */
        std::optional<u_int32_t> maxColumns;

        viewer(utils::file File);
        ~viewer();
        
        viewer& onRefresh();
        viewer& onResize();

        int getColumnCount();
    };
}
