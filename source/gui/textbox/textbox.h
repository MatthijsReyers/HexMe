#pragma once

#include <string>
#include <ncurses.h>

namespace gui
{
    class textbox
    {
    private:
        const static int ESCAPE = 27;
        const static int BACKSPACE = 127;
        const static int CRTL_BACKSPACE = 23;
        const static int DELETE = 330;

        WINDOW* window;
        std::string text;
        unsigned long int cursorPosition = 0;

        void moveCursor(const int direction);
        void addChar(const char character);

    public:
        textbox();
        ~textbox();

        bool focus = false;

        textbox& onRefresh();
        textbox& onInput(const int key);
        textbox& onResize();

        std::string& getText();
        textbox& setText(const std::string text);
        textbox& setText(const char* text);
        textbox& clearText();
    };
}
