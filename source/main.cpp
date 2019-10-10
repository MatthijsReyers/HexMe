
// 10-09-2019 Matthijs Reyers
// 
// Compile command:
// g++ -I ./ -o hexme main.cpp -lncurses

// #include <string>
// #include <thread>
// #include <stdio.h>

// Main headers
#include <curses.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <charconv>
#include <cmath>

// Custom headers
#include <hdetect.h>
#include <argparse.h>

// Main variables
int winX; int winY;
int scrX; int scrY;
int activeX = 0; 
int activeY = 0;
int activeRow = 0;
int collumns;
int fileHeaderLen = 0;
std::string fileHeader;
bool running = true;
int key;
int absoluteIndex;

const char intToChar(int &inNumber)
{
    // Once the file ends, just print spaces.
    if (inNumber < 0) return ' ';

    // Invisible or non char, just print a dot.
    else if (inNumber >= 127 || inNumber <= 32) return '.';

    // Convert int to char.
    else return (char)inNumber;
}

const char* intToHexStr(int inNumber, int padding)
{
    // Just catch -1 drawing, which should not happen.
    if (inNumber < 0) return "  ";

    // Actually convert the byte to a string.
    std::stringstream stream;
    int counter = 0;
    for (int i = 0; i < padding; i++)
    {
        if (inNumber < counter) {stream << "0";}
        counter = counter + 16 * std::pow(16, i);
    }
    stream << std::hex << inNumber;
    std::string result(stream.str());
    const char* out = result.c_str();
    return out;
}

int colorByte(int byte)
{
    if (byte == 0) return 1; // RED
    else if (byte <= 32) return 3; // GREEN
    else if (byte >= 127) return 2; // BLUE
    else return 0; // NO COLOR
}

void winResize(int &winX, int &winY, int &srcX, int &srcY, int &collumns, int &activeX, int &activeY, int &activeRow, int &absoluteIndex)
{
    // Get size of terminal to get max rows.
    // ------------------------------------------------------
    int x, y;
    getmaxyx(stdscr, y, x);
    srcX = x; srcY = y;
    
    // Calculate the amount of collumns that can be shown on
    // screen (maxes out at 6 collumns).
    // ------------------------------------------------------
    int lineNrs = 6 + 2 + 1;
    int collumn = (8 * 3 + 2) + (8 + 2 + 1);
    int collumnCount = 0;
    int lefoverX = srcX - 2 - lineNrs;
    while (lefoverX >= collumn && collumnCount != 6)
    {
        lefoverX = lefoverX - collumn;
        collumnCount++;
    }
    
    // Finally put values into pointers.
    winX = lineNrs + (collumnCount * collumn);
    winY = scrY - 2;
    collumns = collumnCount;

    // Now set the cursor to the proper location again as not to change bytes.
    // ------------------------------------------------------
    activeX = (absoluteIndex - (activeRow * collumns * 8)) / (8 * collumns);
    activeX = (absoluteIndex - (activeRow * collumns * 8)) % (8 * collumns);
}

void drawScreen(int &winX, int &winY, int &collumns, char* &fileName, std::string &fileHeader)
{
    mvaddstr(0,0,fileName);

    // Top & bottom lines.
    for (int i = 0; i != winX; i++)
    {
        mvaddstr(1, i, "═");
        mvaddstr(winY, i, "═");
    }

    // Side lines.
    for (int y = 1; y != winY; y++)
    {
        // Outerbox lines.
        mvaddstr(y, 0, "║");
        mvaddstr(y, winX, "║");
        mvaddstr(y, 0, "║");
        mvaddstr(y, winX, "║");

        // Line numbers.
        mvaddstr(y, 9, "║");

        // Hex rows
        mvaddstr(y, 11 + collumns * (8 * 3) + (collumns - 1) * 2, "║");

        // Line numbers.
        mvaddstr(y, 9, "║");

        // Hex rows
        mvaddstr(y, 11 + collumns * (8 * 3) + (collumns - 1) * 2, "║");

        // Inner hex rows
        for (int c = 1; c < collumns; c++)
        {
            mvaddstr(y,(9 + c * 26),"│");
            mvaddstr(y,(9 + collumns * 26 + c * 11),"│");
        }
    }

    // Connecting pieces.
    mvaddstr(1,9,"╦");
    mvaddstr(1,11 + collumns * (8 * 3) + (collumns - 1) * 2,"╦");
    mvaddstr(winY,9,"╩");
    mvaddstr(winY,11 + collumns * (8 * 3) + (collumns - 1) * 2,"╩");
    
    // Inner connecting pieces.
    for (int c = 1; c < collumns; c++)
    {
        mvaddstr(1,(9 + c * 26),"╤");
        mvaddstr(1,(9 + collumns * 26 + c * 11),"╤");
    
        mvaddstr(winY,(9 + c * 26),"╧");
        mvaddstr(winY,(9 + collumns * 26 + c * 11),"╧");
    }

    // Corner pieces.
    mvaddstr(1,0,"╔");
    mvaddstr(1,winX,"╗");
    mvaddstr(winY,0,"╚");
    mvaddstr(winY,winX,"╝");
}

void drawLineNr(int &srcX, int &srcY, int &Collumns, int &activeRow)
{
    int tempNr = activeRow; // * Collumns * 8;
    for (int i = 0; i < (scrY - 4); i++)
    {
        mvaddstr(2+i,2,intToHexStr(tempNr,6));
        tempNr++;
    }
}

void drawBytes(int &collumns, int &winY, std::fstream &file, int fileHeaderLen, int &activeRow, int &activeX, int &activeY)
{
    int byte; int color;
    const char* byteHex;
    const char* byteChar;

    // Seek file read head to right byte.
    // ------------------------------------------------------
    file.clear();
    file.seekg(activeRow * collumns * 8);    

    // Place hex bytes and chars.
    // ------------------------------------------------------
    for (int r = 0; r < (winY - 2); r++)
    {
        for (int c = 0; c < collumns; c++)
        {
            for (int i = 0; i < 8; i++)
            {
                // Read byte
                if (file.good()) byte = file.get();
                else byte = -1; 
                byteHex = intToHexStr(byte,2);
                std::string sym(1, intToChar(byte));
                byteChar = sym.c_str();

                // Color byte if it is the selected byte.
                if (activeX == r && activeY == i + c*8) attrset(COLOR_PAIR(4));

                // Special coloring for file header bytes.
                else if ((r + activeRow) * collumns * 8 + 8 * c + i < fileHeaderLen) attrset(COLOR_PAIR(6));

                // Otherwise determine byte color with 'colorByte()' function.
                else attrset(COLOR_PAIR(colorByte(byte)));

                // Place hex byte
                mvaddstr( (2+r), (11 + c*26 + i*3), byteHex);

                if (activeX == r && activeY == i + c*8) attrset(COLOR_PAIR(4));
                else attrset(COLOR_PAIR(5));

                // Place char
                mvaddstr( (2+r), (11 + collumns*26 + c*11 + i), byteChar);
                attrset(COLOR_PAIR(5));
            }   
        }
    }
}

int main(int argc, char* argv[])
{
    // Read filename / check args for mistakes.
    // ------------------------------------------------------
    if (argc > 2 || argc == 1) {printf("ERROR: Please use this syntax: hexyou [filename].\n");exit(1);}
    char* fileName = argv[1];

    // Try to open file stream.
    // ------------------------------------------------------
    std::fstream file(fileName);
    if (!file) {printf("ERROR: Failed to open file.\n");exit(1);}

    // Detect file header.
    // ------------------------------------------------------
    getFileHeader(file, fileHeader, fileHeaderLen);

    // Start ncurses and determine terminal size.
    // ------------------------------------------------------
    setlocale(LC_ALL, "");
    initscr();
    use_default_colors();
    start_color();

    // Set up colours.
    // ------------------------------------------------------
    noecho();
    keypad(stdscr, true);
    init_pair(0, -1, -1);
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    init_pair(5, -1, -1);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);

    // Inital screen rezise and first line draws.
    // ------------------------------------------------------
    absoluteIndex = 0;
    winResize(winX, winY, scrX, scrY, collumns, activeX, activeY, activeRow, absoluteIndex);
    drawScreen(winX, winY, collumns, fileName, fileHeader);

    // Main loops
    // ------------------------------------------------------
    while (running)
    {
        // Start by drawing screen.
        drawLineNr(scrX, scrY, collumns, activeRow);
        drawBytes(collumns, winY, file, fileHeaderLen, activeRow, activeX, activeY);
        refresh();
        
        // Handle user input
        key = getch();
        switch (key)
        {
            // Handle terminal resize events.
            case KEY_RESIZE: 
                absoluteIndex = (activeRow + activeY) * collumns * 8 + (activeX + 1);
                winResize(winX, winY, scrX, scrY, collumns, activeX, activeY, activeRow, absoluteIndex); 
                clear();
                drawScreen(winX, winY, collumns, fileName, fileHeader);
                break;
            
            // Shutdown program when [esc] is pressed.
            //case 27: // == KEY_ESC 
                //running = false;
                //break;

            case KEY_DOWN:
                absoluteIndex = absoluteIndex - (8 * collumns);
                if (activeX <= winY-4) activeX++;
                else activeRow++;
                break;

            case KEY_UP: 
                absoluteIndex = absoluteIndex + (8 * collumns);
                if (activeX > 0) activeX--;
                else if (activeRow > 0) { activeRow--;}
                break;

            case KEY_LEFT: 
                if (activeY > 0) activeY--;
                else if (activeX > 0) {activeX--; activeY = collumns * 8 - 1;}
                break;

            case KEY_RIGHT:
                if (activeY < collumns * 8 - 1) activeY++;
                else if (activeX < 9999) {
                    if (activeX <= winY-4) activeX++;
                    else activeRow++;
                    activeY = 0;}
                break;

            default: break;
        }
    }

    // Cleanup time
    // ------------------------------------------------------
    endwin();
    file.close();
}

// */