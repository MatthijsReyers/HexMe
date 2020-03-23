#include "textbox.h"

namespace gui
{
    textbox::textbox()
    {
        int y = getmaxy(stdscr) - 1;
        int width = getmaxx(stdscr) - 2;
        window = newwin(1, width, y, 0);

        this->onRefresh();
    }

    textbox::~textbox()
    {
        delwin(window);
    }

    textbox& textbox::onRefresh()
    {
        mvwprintw(window, 0, 0, "> ");
        mvwprintw(window, 0, 2, text.c_str());
        wmove(window, 0, 2+cursorPosition);
        wrefresh(window);
        return *this;
    }

    textbox& textbox::onInput(int key)
    {
        if (key >= 32 && key <= 126) {
            text.insert(cursorPosition,{char(key)});
            cursorPosition++;
        }

        else switch (key)
        {
            case ESCAPE:
                this->focus = false;
                break;

            case BACKSPACE:
                if (text.length() > 0 && cursorPosition > 0) {
                    text.erase(cursorPosition-1, 1);
                    if (cursorPosition > 0)
                        cursorPosition--;
                } break;

            case CRTL_BACKSPACE:
                while (text.length() > 0 && cursorPosition > 0 && text[cursorPosition-1] != ' ') {
                    text.erase(cursorPosition-1, 1);
                    if (cursorPosition > 0)
                        cursorPosition--;
                }
                while (text.length() > 0 && cursorPosition > 0 && text[cursorPosition-1] == ' ') {
                    text.erase(cursorPosition-1, 1);
                    if (cursorPosition > 0)
                        cursorPosition--;
                } break;

            case DELETE:
                if (text.length() > 0 && cursorPosition < text.length())
                    text.erase(cursorPosition, 1);
                break;

            case KEY_HOME:
                cursorPosition = 0;
                break;

            case KEY_END:
                cursorPosition = text.length();
                break;
            
            case KEY_LEFT:
                if (cursorPosition > 0)
                    cursorPosition--;
                break;
            
            case KEY_RIGHT:
                if (cursorPosition < text.size())
                    cursorPosition++;
                break;
        }
        return *this;
    }

    textbox& textbox::onResize()
    {
        int y = getmaxy(stdscr) - 1;
        int width = getmaxx(stdscr) - 2;
        wresize(window, 1, width);
        wmove(window, y, 0);
        return *this;
    }

    std::string& textbox::getText()
    {
        return this->text;
    }

    textbox& textbox::setText(const std::string t)
    {
        this->text = t;
        return *this;
    }

    textbox& textbox::setText(const char* t)
    {
        this->text = std::string(t);
        return *this;
    }

    textbox& textbox::clearText()
    {
        cursorPosition = 0;
        text.clear();
        wclear(window);
        this->onRefresh();
        return *this;
    }
}
