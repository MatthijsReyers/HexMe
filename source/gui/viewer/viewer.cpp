#include "viewer.h"

namespace gui
{
    viewer::viewer(utils::file File) : file(File)
    {
        // Create window
        this->window = newwin(50, 50, 1, 0);

        // Initial drawing of window.
        this->onResize();

        // Initial refresh of window.
        this->onRefresh();
    }

    viewer::~viewer()
    {
        delwin(window);
    }

    viewer& viewer::onRefresh()
    {
        for (int r = 0; r < rows; r++)
            this->drawRow(r);
        wrefresh(window);
        return *this;
    }

    viewer& viewer::onResize()
    {
        // Clear window.
        wclear(window);

        // Calculate new row & column count.
        this->columns = (getmaxx(stdscr) - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;
        this->rows = getmaxy(stdscr) - 4;

        // Calculate new size.
        this->width = LINENUMBERSWIDTH + columns * COLUMNWIDTH + 1; 
        this->height = getmaxy(stdscr) - 2;

        // Resize window.
        wresize(window, height, width);

        // Draw window contents.
        this->drawBorders();
        for (int r = 0; r < rows; r++)
            this->drawRow(r);

        return *this;
    }

    viewer& viewer::onMoveCursor(int direction)
    {
        return *this;
    }

    int viewer::getByteColor(byte b)
    {
        if (b == 0) return 1;       // RED
        else if (b <= 32) return 3; // GREEN
        else if (b >= 127) return 2;// BLUE
        else return 0;              // NO COLOR
    }

    void viewer::drawRow(unsigned long long r)
    {
        std::stringstream ss;

        // Row number.
        int index = rowIndex + (r * columns * 8);
        ss << std::hex << index;
        std::string zeros = "00000000";
        zeros.erase(zeros.length() - ss.str().length());
        zeros.append(ss.str());
        mvwprintw(window, 1+r, 2, zeros.c_str());

        // Set cursor to start of row.
        file.moveCursor(r);

        for (int c = 0; c < columns; c++)
            for (int b = 0; b < 8; b++)
            {
                // Check if there are any bytes left in the buffer.
                if (file.getBytesAfterCursor() == 0) {
                    mvwprintw(window, 1+r, 13 + c*26 + b*3, "  ");
                    mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, " ");
                }
                else {
                    // Get byte & increment file cursor.
                    byte current = file.getCurrentByte();
                    file.incCursor();

                    // Get appropriate color for byte.
                    attrset(COLOR_PAIR(getByteColor(current)));

                    // Draw hex representation of byte.
                    mvwprintw(window, 1+r, 13 + c*26 + b*3, utils::byteToHexString(current).c_str());

                    // Draw char representation of byte.
                    if (current < 127 && current > 32) mvwaddch(window, 1+r, 13 + columns*26 + c*11 + b, current);
                    else mvwprintw(window, 1+r, 13 + columns*26 + c*11 + b, ".");
                }
            }
        
        for (int c = 1; c < columns; c++) {
            mvwprintw(window, 1+r, 11 + c*26, "│");
            mvwprintw(window, 1+r, 11 + columns*26 + c*11, "│");
        }

        mvwprintw(window, 1+r, 11 + columns*26, "║");
        mvwprintw(window, 1+r, 11, "║");
    }

    void viewer::drawBorders()
    {
        const static char* horizontal = "═";
        const static char* vertical = "║";

        const static char* cornerTL = "╔";
        const static char* cornerTR = "╗";
        const static char* cornerBL = "╚";
        const static char* cornerBR = "╝";

        const static char* tBracketD = "╦";
        const static char* tBracketU = "╩";

        const static char* tThinD = "╤";
        const static char* tThinU = "╧";

        for (int y = 0; y < height-1; y++) {
            mvwprintw(window, y, 0, vertical);
            mvwprintw(window, y, width-2, vertical);
        }

        for (int x = 0; x < width-1; x++) {
            mvwprintw(window, 0, x, horizontal);
            mvwprintw(window, height-1, x, horizontal);
        }

        mvwprintw(window, 0, 0, cornerTL);
        mvwprintw(window, 0, width-2, cornerTR);
        mvwprintw(window, height-1, 0, cornerBL);
        mvwprintw(window, height-1, width-2, cornerBR);

        mvwprintw(window, 0, 11, tBracketD);
        mvwprintw(window, height-1, 11, tBracketU);

        mvwprintw(window, 0, 11 + columns * 26, tBracketD);
        mvwprintw(window, height-1, 11 + columns * 26, tBracketU);

        for (int c = 1; c < columns; c++) {
            mvwprintw(window, 0, 11 + c * 26, tThinD);
            mvwprintw(window, height-1, 11 + c * 26, tThinU);
            mvwprintw(window, 0, 11 + columns * 26 + c * 11, tThinD);
            mvwprintw(window, height-1, 11 + columns * 26 + c * 11, tThinU);
        }
    }

    unsigned long int getRowFromIndex(const unsigned long long index)
    {
        return index;
    }
}