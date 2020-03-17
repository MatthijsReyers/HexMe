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
        wrefresh(window);
        return *this;
    }

    viewer& viewer::onResize()
    {
        // Clear window.
        wclear(window);

        // Calculate new size.
        this->width = getmaxx(stdscr); 
        this->height = getmaxy(stdscr) - 2;

        // Resize window.
        wresize(window, height, width);

        // Calculate new column count.
        this->columns = (width - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;
        this->rows = height - 4;

        // Draw window contents.
        this->drawBorders();

        return *this;
    }


    void viewer::drawRow(long r)
    {

    }

    void viewer::drawBorders()
    {
        // Draw the surrounding box.
        box(window, '|', '-');
        mvwprintw(window, 2, 2, "-----");
    }

    unsigned long int getRowFromIndex(const unsigned long long index)
    {
        return index;
    }



    // viewer& viewer::onRefresh()
    // {
    //     // Clear window contents.
    //     wclear(this->window);

    //     return *this;
    // }
}