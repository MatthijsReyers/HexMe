
// 10-09-2019 Matthijs Reyers
// 
// Compile command:
// g++ -I ./ -o hexme main.cpp

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

// Main variables.
int winX; int winY;
int scrX; int scrY;
int activeX = 0; 
int activeY = 0;
int collumns;
int topNr = 0;
bool running = true;

void convertActive(int &activeX, int &activeY)
{
    //
}

const char intToChar(int &inNumber)
{
    // Once the file ends, just print spaces.
    if (inNumber == -1) return ' ';

    // Invisible or non char, just print a dot.
    else if (inNumber >= 127 || inNumber <= 32) return '.';

    // Convert int to char.
    else return (char)inNumber;
}

const char* intToHexStr(int inNumber, int padding)
{
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

void winResize(int &winX, int &winY, int &srcX, int &srcY, int &collumns)
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

void drawLineNr(int &topNr, int &srcX, int &srcY)
{
    int tempNr = topNr;
    for (int i = 0; i < (scrY - 4); i++)
    {
        mvaddstr(2+i,2,intToHexStr(tempNr,6));
        tempNr++;
    }
}

void drawBytes(int &collumns, int &winY, std::fstream &file)
{
    int activeLine = 0;
    int byte;
    const char* byteHex;
    const char* byteChar;

    // Seek file read head to right byte.
    // ------------------------------------------------------
    file.seekg(activeLine * collumns * 8);    

    // Place hex bytes and chars.
    // ------------------------------------------------------
    for (int r = 0; r < (winY - 2); r++)
    {
        for (int c = 0; c < collumns; c++)
        {
            for (int i = 0; i < 8; i++)
            {
                // Read byte
                byte = file.get();
                byteHex = intToHexStr(byte,2);
                std::string sym(1, intToChar(byte));
                byteChar = sym.c_str();

                // Place
                mvaddstr( (2+r), (11 + c*26 + i*3), byteHex);
                mvaddstr( (2+r), (11 + collumns*26 + c*11 + i), byteChar);
            }   
        }
    }
}

// int main()
// {
//     std::cout << intToHexStr(555,2) << std::endl;
// }


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
    std::string fileHeader = getFileHeader(file);

    // Start ncurses and determine terminal size.
    // ------------------------------------------------------
    setlocale(LC_ALL, "");
    initscr();
    winResize(winX, winY, scrX, scrY, collumns);
    int key;

    // Main loops
    // ------------------------------------------------------
    while (running)
    {
        // Clean up old stuff
        clear();

        // Start by drawing screen
        drawScreen(winX, winY, collumns, fileName, fileHeader);
        drawLineNr(topNr, scrX, scrY);
        drawBytes(collumns, winY, file);
        refresh();
        
        // Handle user input
        key = getch();
        switch (key)
        {
            // Handle terminal resize events.
            case KEY_RESIZE: 
                winResize(winX, winY, scrX, scrY, collumns); 
                convertActive(activeX, activeY);
                break;
            
            // Shutdown program when [esc] is pressed.
            case 27: // == KEY_ESC 
                running = false;
                break;

            case KEY_UP: break;
            case KEY_DOWN: break;
            case KEY_LEFT: break;
            case KEY_RIGHT:break;
            default: break;
        }
    }

    // Cleanup time
    // ------------------------------------------------------
    endwin();
    file.close();
}

// */