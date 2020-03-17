#include "app.h"

app::app(utils::file& File, utils::arguments& Args) : file(File), args(Args)
{
    // Setup nCurses.
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    noecho();

    // Initial refresh.
    refresh();

    // Setup all UI elements.
    cmdPromt = new gui::textbox();
    hexView = new gui::viewer(this->file);

    // Calculate intial window values.
    this->onResize();
}

app& app::run()
{
    this->running = true;
    while (this->running)
    {
        int input = getch();
        if (input == KEY_RESIZE)
            this->onResize();

        else if (cmdPromt->hasFocus())
        {
            if (input == 10 /*KEY_ENTER*/) {
                this->executeCmd(cmdPromt->getText());
                cmdPromt->clearText();
            }
            else cmdPromt->onInput(input);
        }

        else switch (input)
        {
            case KEY_DOWN:
                this->onMoveCursor(columns*8);
                break;
            case KEY_UP:
                this->onMoveCursor(columns*-8);
                break;
            case KEY_LEFT:
                this->onMoveCursor(-1);
                break;
            case KEY_RIGHT:
                this->onMoveCursor(1);
            default:
                cmdPromt->setFocus(true);
                cmdPromt->onInput(input);
                break;
        }
    }

    // Return reference to self.
    return (*this);
}

app& app::close()
{
    // Set terminal back to normal mode.
    echo();
    endwin();

    // Return reference to self.
    return (*this);
}

app& app::onResize()
{
    // Update all window content.
    hexView->onResize();
    cmdPromt->onResize();

    // Refresh all windows.
    hexView->onRefresh();
    cmdPromt->onRefresh();

    // Recalculate column count.
    this->columns = (getmaxy(stdscr) - LINENUMBERSWIDTH - 1) / COLUMNWIDTH;

    // Return reference to self.
    return (*this);
}

app& app::onMoveCursor(int n)
{
    // Update all windows.
    hexView->onRefresh();
    cmdPromt->onRefresh();

    // Return reference to self.
    return (*this);
}

app& app::executeCmd(const std::string& cmd)
{
    return (*this);
}
