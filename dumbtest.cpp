#include <string>
#include <ncurses.h>
#include <locale.h>

class viewer
{
private:
    const static int LINENUMBERSWIDTH = 8 + 2 + 2;
    const static int COLUMNWIDTH = (8 * 3 + 2) + (8 + 3);

    WINDOW* window;

    int width, height;
    int columns, rows;
    
    int rowIndex = 0;

    void drawRow(long r);
    void drawBorders();

    unsigned long int getRowFromIndex(const unsigned long long index);
    unsigned long int getYFromIndex(const unsigned long long index);

public:
    viewer();
    ~viewer();

    viewer& onRefresh();
    viewer& onResize();
};

viewer::viewer()
{
    // Calculate window size.
    this->width = getmaxy(stdscr); 
    this->height = getmaxx(stdscr) - 2;

    // Create window
    // this->window = newwin(height, width, 1, 0);
    this->window = newwin(10, 10, 0, 0);
    // mvprintw(0, 0, "creating new window...");
    // refresh();

    // Initial drawing of window.
    this->onResize();
}

viewer::~viewer()
{
    delwin(window);
}

viewer& viewer::onRefresh()
{
    mvwprintw(window, 0, 0, "<=_=>");
    mvwprintw(window, 5, 5, "<=_=>");
    wrefresh(window);
    return *this;
}

viewer& viewer::onResize()
{
    return *this;
}


void viewer::drawRow(long r)
{

}

void viewer::drawBorders()
{
    // Draw the surrounding box.
    // box(window);

    mvwprintw(this->window, 5, 5, "==>");
}

unsigned long int getRowFromIndex(const unsigned long long index)
{
    return index;
}

int main(int argc, char const *argv[])
{
    // Setup nCurses.
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    noecho();

    // Create hexview object.
    viewer* hexView;
    hexView = new viewer();

    hexView->onResize();
    hexView->onRefresh();

    while (true)
    {
        int x = getch();
        hexView->onRefresh();
    }

    // Delete object at pointer.
    delete hexView;

    // Set terminal back to normal mode.
    echo();
    endwin();
}
