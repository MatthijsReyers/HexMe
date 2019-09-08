
// 18-05-2019 Matthijs Reyers
// 
// Compile command:
// g++ -I ./ -o hexme main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <stdio.h>

// Custom headers.
#include <argparser.h>
#include <colours.h>

void printLineNum(int &counter, int rows)
{
    std::string filler;
    if (counter < 16) filler = "00000";
    else if (counter < 256) filler = "0000";
    else if (counter < 4096) filler = "000";
    else if (counter < 65536) filler = "00";
    else if (counter < 1048576) filler = "0";
    std::cout << std::hex << filler << counter << " ║ ";
    counter = counter + (8 * rows);
}

// Function for loading bytes into an int buffer.
// ----------------------------------------------------------
void loadBuffer(std::ifstream &file, int (&buffer)[32], int rows)
{
    int Max = rows * 8;
    for (int i = 0; i < Max; i++)
    {
        if (file.good()) buffer[i] = file.get();
        else buffer[i] = 999;
    }
}

// 
// ----------------------------------------------------------
std::string getHexColour(int hex)
{
    if (hex == 0) return blue;          // NULL byte
    else if (hex < 32) return green;    // non character bytes
    else if (hex > 126) return red;     // bytes outside ascii range
    else return reset;                  // character bytes
}

// Function for outputing int buffer to console in hex form.
// ----------------------------------------------------------
void printRowHex(int (&buffer)[32], int row)
{
    std::string colour;
    int rangeMin = 8 * row;
    int rangeMax = 8 * (row + 1);
    for (int i = rangeMin; i < rangeMax; i++)
    {   
        if (buffer[i] != 999 && buffer[i] != -1)
        {
            colour =  getHexColour(buffer[i]);
            if (buffer[i] < 16) std::cout << colour << "0" << std::hex << buffer[i] << " ";
            else std::cout << colour << std::hex << buffer[i] << " ";
        }
        // If the file has ended just print spaces.
        else std::cout << "   ";
    }
    std::cout << reset << "║ "; 
}

// Function for outputing int buffer to console in char form.
// ----------------------------------------------------------
void printRowChar(int (&buffer)[32], int row)
{
    char converted;
    int rangeMin = 8 * row;
    int rangeMax = 8 * (row + 1);
    for (int i = rangeMin; i < rangeMax; i++)
    {
        // Check if file has not ended and char is not an endl.
        if (buffer[i] > 126 || buffer[i] < 32)
        {
            // If the file has ended or we find an invisible char, just print a space.
            std::cout << " ";
        }
        else
        {
            // Convert int to char and print.
            converted = (char)buffer[i];
            if (converted == 10) std::cout << "_";
            else std::cout << converted;
        }   
    }
    std::cout << " ║ ";
}

int main(int argc, char* argv[])
{
    // Parse command line arguments.
    // ------------------------------------------------------
    int rows = 2;
    char* fileUrl;
    bool lineNums = true;
    parseArgs(argc, argv, rows, fileUrl, lineNums);
    
    // Open file.
    // ------------------------------------------------------
    std::ifstream file;
    file.open(fileUrl);
    if (!file) {printf("ERROR: The specified file does not exist.\n"); return 1;}

    // Generate top.
    // ------------------------------------------------------
    std::cout << "╔";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╦";         // Line numbers
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "═══════════════════════" << "╦";}// Hex rows
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════";                        // Char rows
        if (i != (rows -1)) std::cout << "╦";}
    std::cout << "╗" << std::endl;                              // Cornerpiece and line break

    // Generate Rows
    // ------------------------------------------------------
    int buffer[32];
    int rowCount = 0;
    while (file.good())
    {
        // Starting line.
        std::cout << "║ ";

        // Print row numbers.
        if (lineNums) printLineNum(rowCount, rows);

        // Load bytes into buffer.
        loadBuffer(file, buffer, rows);

        // Print hex rows.
        for (int i = 0; i < rows; i++) printRowHex(buffer, i);
        
        // Print char rows.
        for (int i = 0; i < rows; i++) printRowChar(buffer, i);
        
        // Line break
        std::cout << std::endl;
    }

    // Generate bottom.
    // ------------------------------------------------------
    std::cout << "╚";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╩";         // Line numbers
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "═══════════════════════" << "╩";}// Hex rows
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════";                        // Char rows
        if (i != (rows -1)) std::cout << "╩";}
    std::cout << "╝" << std::endl;                              // Cornerpiece and final line break

    // Cleanup time
    // ------------------------------------------------------
    file.close();
    return 0;
}
