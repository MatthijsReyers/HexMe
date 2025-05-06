#include "./message-box-okay.hpp"

namespace gui
{
    MessageBoxOkay::MessageBoxOkay(HexMeApp* app, const std::vector<std::string> t): text(t), app(app)
    {
        this->height = text.size() + 3;

        // Find the widest line to set the width of the msg box to.
        for (auto line : text)
            if (width < int(line.size()) + 4)
                this->width = line.size() + 4;

        this->window = newwin(height, width, 0, 0);
    }

    MessageBoxOkay::MessageBoxOkay(HexMeApp* app, const std::string t): app(app)
    {
        this->text.push_back(t);
        this->height = 4;
        this->width = t.length() + 4;
        this->window = newwin(height, width, 0, 0);
    }

    MessageBoxOkay::~MessageBoxOkay()
    {
        delwin(window);
    }

    MessageBoxOkay& MessageBoxOkay::display()
    {
        this->onResize();

        // Wait for user input.
        auto input = getch();

        // All other user input dismisses the message box, but a terminal resize just means we
        // should recompute the message box position and display it again.
        if (input == KEY_RESIZE) {
            this->app->onResizeTerminal();
            this->display();
        }

        return *this;
    }

    MessageBoxOkay& MessageBoxOkay::onResize()
    {
        x = (getmaxx(stdscr) - 13) / (26+11);
        x = (x * (26+11)) + 12;
        x = (x - width) / 2;

        y = (getmaxy(stdscr) - height) / 2;

        wclear(window);
        mvwin(window, y, x);
        
        this->onRefresh();

        return *this;
    }

    MessageBoxOkay& MessageBoxOkay::onRefresh()
    {
        const static char* horizontal = "═";
		const static char* vertical = "║";

		const static char* cornerTL = "╔";
		const static char* cornerTR = "╗";
		const static char* cornerBL = "╚";
		const static char* cornerBR = "╝";

        for (int x = 1; x < width-1; x++) {
            mvwprintw(window, 0, x, horizontal);
            mvwprintw(window, height-1, x, horizontal);
        }

        for (int y = 1; y < height-1; y++) {
            mvwprintw(window, y, 0, vertical);
            mvwprintw(window, y, width-1, vertical);
        }

        mvwprintw(window, 0, 0, cornerTL);
		mvwprintw(window, 0, width-1, cornerTR);
		mvwprintw(window, height-1, 0, cornerBL);
		mvwprintw(window, height-1, width-1, cornerBR);

        for (std::size_t i = 0; i < text.size(); i++)
            mvwprintw(window, 1+i, 2, text[i].c_str());

        wattron(window, COLOR_PAIR(9));
        mvwprintw(window, height-2, (width / 2)-2, "[OK]");
        wattroff(window, COLOR_PAIR(9));

        int x, y;
        getyx(stdscr, y, x);
        move(y,x);

        wrefresh(window);
        return *this;
    }
}