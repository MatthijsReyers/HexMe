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

        textbox& onRefresh() noexcept;
        textbox& onInput(const int key) noexcept;
        textbox& onResize() noexcept;

        std::string& getText() noexcept;
        textbox& setText(const std::string text) noexcept;
        textbox& setText(const char* text) noexcept;
        textbox& clearText() noexcept;
    };
}
