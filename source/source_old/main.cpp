
// 07-09-2019 Matthijs Reyers
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
#include <headerdetect.h>

void printLineNum(int &counter, int columns)
{
    std::string filler;
    if (counter < 16) filler = "00000";
    else if (counter < 256) filler = "0000";
    else if (counter < 4096) filler = "000";
    else if (counter < 65536) filler = "00";
    else if (counter < 1048576) filler = "0";
    std::cout << std::hex << filler << counter << " ║ ";
    counter = counter + (8 * columns);
}

// Function for loading bytes into an int buffer.
// ----------------------------------------------------------
void loadBuffer(std::ifstream &file, int (&buffer)[32], int columns)
{
    int Max = columns * 8;
    for (int i = 0; i < Max; i++)
    {
        if (file.good()) buffer[i] = file.get();
        else buffer[i] = -1;
    }
}

// 
// ----------------------------------------------------------
std::string getHexColour(int hex)
{
    if (hex == 0) return red;          // NULL byte
    else if (hex < 32) return green;    // non character bytes
    else if (hex > 126) return blue;     // bytes outside ascii range
    else return reset;                  // character bytes
}

// Function for outputing int buffer to console in hex form.
// ----------------------------------------------------------
void printRowHex(int (&buffer)[32], int column)
{
    std::string colour;
    int rangeMin = 8 * column;
    int rangeMax = 8 * (column + 1);
    for (int i = rangeMin; i < rangeMax; i++)
    {   
        if (buffer[i] != -1)
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
void printRowChar(int (&buffer)[32], int column)
{
    char converted;
    int rangeMin = 8 * column;
    int rangeMax = 8 * (column + 1);
    for (int i = rangeMin; i < rangeMax; i++)
    {
        if (buffer[i] == -1)
        {
            // If the file has ended print a space.
            std::cout << " ";
        }
        else if (buffer[i] >= 127 || buffer[i] <= 32)
        {
            // If it is an invisible or non char, just print a dot.
            std::cout << ".";
        }
        else
        {
            // Convert int to char and print.
            converted = (char)buffer[i];
            std::cout << converted;
        }   
    }
    std::cout << " ║ ";
}

int main(int argc, char* argv[])
{
    // Main variables.
    // ------------------------------------------------------
    int columns = 2;
    char* fileUrl;
    bool lineNums = true;
    std::ifstream file;
    std::string fileheader;
    int buffer[32];
    int columnCount = 0;

    // Parse command line arguments.
    // ------------------------------------------------------
    parseArgs(argc, argv, columns, fileUrl, lineNums);
    
    // Open file.
    // ------------------------------------------------------
    file.open(fileUrl);
    if (!file) {printf("ERROR: The specified file does not exist.\n"); return 1;}

    // Generate top-text.
    // ------------------------------------------------------
    std::cout << "Showing file: " << fileUrl;
    loadBuffer(file, buffer, 4);
    file.seekg(0);
    fileheader = getFileHeader(buffer);
    if (fileheader != "") std::cout << " | Found file header: " << fileheader;
    std::cout << std::endl;
    

    // Generate top-lines.
    // ------------------------------------------------------
    std::cout << "╔";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╦";         // Line numbers
    for (int i = 0; i < columns; i++) {
        std::cout << "══" << "═══════════════════════" << "╦";} // Hex columns
    for (int i = 0; i < columns; i++) {
        std::cout << "══" << "════════";                        // Char columns
        if (i != (columns -1)) std::cout << "╦";}
    std::cout << "╗" << std::endl;                              // Cornerpiece and line break

    // Generate columns
    // ------------------------------------------------------
    while (file.good())
    {
        // Load next bytes into buffer.
        loadBuffer(file, buffer, columns);

        // Starting line.
        std::cout << "║ ";

        // Print row numbers.
        if (lineNums) printLineNum(columnCount, columns);

        // Print hex columns.
        for (int i = 0; i < columns; i++) printRowHex(buffer, i);
        
        // Print char columns.
        for (int i = 0; i < columns; i++) printRowChar(buffer, i);
        
        // Line break
        std::cout << std::endl;
    }

    // Generate bottom.
    // ------------------------------------------------------
    std::cout << "╚";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╩";         // Line numbers
    for (int i = 0; i < columns; i++) {
        std::cout << "══" << "═══════════════════════" << "╩";} // Hex columns
    for (int i = 0; i < columns; i++) {
        std::cout << "══" << "════════";                        // Char columns
        if (i != (columns -1)) std::cout << "╩";}
    std::cout << "╝" << std::endl;                              // Cornerpiece and final line break

    // Cleanup time
    // ------------------------------------------------------
    file.close();
}
