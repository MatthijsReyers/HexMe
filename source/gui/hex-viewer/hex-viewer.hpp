#pragma once

#include "./../file/file.hpp"
#include "ncurses.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include <optional>
#include <memory>

namespace gui
{
    class HexViewer
    {
    private:
        const static int LINE_NUMBERS_WIDTH = 8 + 2 + 2;
        const static int COLUMN_WIDTH = (8 * 3 + 2) + (8 + 3);

        std::weak_ptr<File> file;

        WINDOW* window;

        u_int32_t width, height;
        u_int32_t columns, rows; 

        u_int64_t topRow = 0;
        u_int64_t headerLength = 0;

        void drawRow(u_int64_t r);
        void drawBorders();

        int getByteColor(u_int8_t b, u_int64_t index, u_int64_t cursor);

        u_int64_t getYofCursor(const u_int64_t cursor);
        u_int64_t getXofCursor(const u_int64_t cursor);
        
	    std::string byteToHexString(const u_int8_t b);
    
    public:
        /**
         * Maximum amount of columns the hex view is allowed to display, note that the UI will not
         * reflect changes to this value until onRefresh() is called.
         */
        std::optional<u_int32_t> maxColumns;

        HexViewer(std::weak_ptr<File> file);
        ~HexViewer();
        
        HexViewer& onRefresh();
        HexViewer& onResize();

        int getColumnCount();
    };
}
