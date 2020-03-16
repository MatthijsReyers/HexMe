#include <locale.h>
#include <curses.h>
#include <string>
#include <sstream>

#define KEY_ESCAPE 27
#define KEY_RESIZE 410
#define KEY_ENTER 10
#define KEY_BACKSPACE 127
#define KEY_CRTL_BACKSPACE 23
#define KEY_DELETE 330
#define KEY_CRTL_DELETE 330

class textbox
{
    private:
        WINDOW* window;
        std::string text;
        int cursorPosition;

        void moveCursor(int direction);
        void addChar(char character);

    public:
        textbox();
        ~textbox();

        textbox& onRefresh();
        textbox& onInput(int key);
        textbox& onResize();

        std::string& getText();
        textbox& setText(const std::string text);
        textbox& setText(const char* text);
};

textbox::textbox()
{
    int y = getmaxy(stdscr) - 1;
    int width = getmaxx(stdscr) - 2;
    window = newwin(1, width, y, 0);
}

textbox::~textbox()
{
    delwin(window);
}

textbox& textbox::onRefresh()
{
    wclear(window);
    mvwprintw(window, 0, 0, "> ");
    mvwprintw(window, 0, 2, text.c_str());
    mvcur(0,2+text.length(),0,1+text.length());
    wrefresh(window);

    std::stringstream ss;
    ss << cursorPosition;
    mvaddstr(15,15,"    ");
    mvaddstr(15,15,ss.str().c_str());
    return *this;
}

textbox& textbox::onInput(int key)
{
    if (key >= 32 && key <= 126)
    {
        text.insert(cursorPosition,{char(key)});
        cursorPosition++;
    }

    else switch (key)
    {
        case KEY_ENTER:
            text.erase();
            cursorPosition = 0;
            break;

        case KEY_BACKSPACE:
            if (text.length() > 0 && cursorPosition > 0) {
                text.erase(cursorPosition-1, 1);
                if (cursorPosition > 0)
                    cursorPosition--;
            } break;

        case KEY_DELETE:
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
    this->onRefresh();
    return *this;
}

textbox& textbox::onResize()
{
    int y = getmaxy(stdscr) - 1;
    int width = getmaxx(stdscr) - 2;
    wresize(window, 1, width);
    wmove(window, y, 0);
    this->onRefresh();
    return *this;
}

int main(int argc, char const *argv[])
{
    // Setup nCurses.
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, true);
    noecho();
    use_default_colors();
    start_color();

    textbox inputfield = textbox();

    auto running = true;
    while (running)
    {
        int key = getch();
        std::stringstream ss;
        ss << key;
        mvaddstr(0,0,ss.str().c_str());
        switch (key)
        {
            case KEY_RESIZE: 
                mvaddstr(3,3,"Just resized...");
                inputfield.onResize();
                break;

            case KEY_ESCAPE: 
                running = false;
                break;

            default:
                inputfield.onInput(key);
                break;
        }
    }
    
    // Set terminal back to normal mode.
    echo();
    endwin();
}
