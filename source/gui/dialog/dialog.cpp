#include "./dialog.hpp"

const static int ESCAPE = 27;
const static int ENTER = 13;

namespace gui
{
    Dialog::Dialog(
        const std::vector<std::string> text, 
        const std::vector<std::string> buttons
    ): text(text), buttons(buttons)
    {
        this->height = text.size() + 2;

        if (this->buttons.size() > 0) {
            this->height += 1;
        }

        // Find the widest line to set the width of the msg box to.
        for (auto line : text) {
            if (this->width < line.size() + 4) {
                this->width = line.size() + 4;
            }
        }

        auto buttonsWidth = this->buttons.size();
        for (auto button : this->buttons) {
            buttonsWidth += button.size() + 3;
        }
        if (this->width < buttonsWidth) {
            this->width = buttonsWidth;
        }

        this->window = newwin(height, width, 0, 0);
    }

    Dialog::Dialog(const std::vector<std::string> t, std::string b)
        : Dialog(t, std::vector({ b })) {}

    Dialog::Dialog(std::string t, const std::vector<std::string> b)
        : Dialog(std::vector({ t }), b) {}

    Dialog::Dialog(std::string t, std::string b)
        : Dialog(std::vector({ t }), std::vector({ b })) {}

    Dialog::~Dialog()
    {
        delwin(this->window);
    }

    std::optional<size_t> Dialog::display()
    {
        this->choice = 0;
        if (this->buttons.size() > 0) {
            choice = std::max(this->defaultChoice, this->buttons.size() - 1);
        }

        bool open = true;
        while (open) 
        {
            this->onResize();

            // Wait for user input.
            auto input = getch();

            switch (input)
            {
            case KEY_RESIZE: {
                    auto app = getApp();
                    app->onResizeTerminal();
                } break;
            
            case ESCAPE:
                if (!this->forceChoice || this->buttons.size() == 0) {
                    return {};
                }
                break;

            case KEY_LEFT:
                if (choice > 0) {
                    choice -= 1;
                }
                break;

            case KEY_RIGHT:
                if (choice + 1 < this->buttons.size()) {
                    choice += 1;
                }
                break;

            case ENTER:
            case KEY_ENTER:
                if (this->buttons.size() > 0) {
                    return choice;
                } 
                return {};
            }
        }
        return {};
    }

    Dialog& Dialog::onResize()
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

    void Dialog::drawBorder() 
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
    }

    void Dialog::drawButtons()
    {
        auto buttonsWidth = this->buttons.size();
        for (auto button : this->buttons) {
            buttonsWidth += button.size() + 2;
        }

        unsigned int x = (width - buttonsWidth) / 2;

        for (size_t i = 0; i < this->buttons.size(); i++)
        {
            x += 1;

            if (i == this->choice) {
                wattron(window, COLOR_PAIR(9));
            }

            auto text = this->buttons[i];
            mvwprintw(window, this->height-2, x, "[");
            x += 1;
            mvwprintw(window, this->height-2, x, text.c_str());
            x += text.size();
            mvwprintw(window, this->height-2, x, "]");
            x += 1;

            if (i == this->choice) {
                wattroff(window, COLOR_PAIR(9));
            }
        }
    }

    Dialog& Dialog::onRefresh()
    {
        this->drawBorder();
        this->drawButtons();        

        for (std::size_t i = 0; i < text.size(); i++)
            mvwprintw(window, 1+i, 2, text[i].c_str());

        int x, y;
        getyx(stdscr, y, x);
        move(y,x);

        wrefresh(window);
        return *this;
    }
}