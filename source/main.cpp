
// 18-05-2019 Matthijs Reyers
// 

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <stdio.h>

int buffer0[8];
int buffer1[8];
int buffer2[8];
int buffer3[8];

// Function for loading 8 chars into an int buffer.
// ----------------------------------------------------------
int* loadBuffer(std::ifstream file)
{
    int outBuffer[8];
    for (int i = 0; i < 8; i++)
    {
        if (file.good()) outBuffer[i] = file.get();
        else outBuffer[i] = NULL;
    }
    return outBuffer;
}

// Function for outputing int buffer to console in hex form.
// ----------------------------------------------------------
void printRowHex(int buffer[])
{
    
}

int main(int argc, char* argv[])
{
    // Check command line arguments
    // ------------------------------------------------------
    if (argc > 2) {printf("ERROR: HexMe only needs one argument.\n"); return 1;}
    if (argc == 1){printf("ERROR: Please specify a file to view.\n"); return 1;}
    
    // Open file.
    // ------------------------------------------------------
    std::ifstream file;
    file.open(argv[1]);
    if (!file) {printf("ERROR: The specified file does not exist.\n"); return 1;}

    // Temporary variables.
    // ------------------------------------------------------
    int rows = 2;
    bool lineNums = true;

    // Generate top.
    // ------------------------------------------------------
    std::cout << "╔";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╦";         // Line numbers
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════════════════════" << "╦";}// Hex rows
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════";                        // Char rows
        if (i != (rows -1)) std::cout << "╦";}
    std::cout << "╗" << std::endl;                              // Cornerpiece and line break

    // Generate Rows
    // ------------------------------------------------------
    int buffer[8];
    char converted;
    while (file.good())
    {
        std::cout << "| ";

        for (int i = 0; i < 8; i++)
        {
            if (buffer[i] != 999)
            {
                if (buffer[i] < 16) std::cout << "0" << std::hex << buffer[i] << " ";
                else std::cout << std::hex << buffer[i] << " ";
            }
            // If the file has ended just print spaces.
            else std::cout << "   ";
        }
        std::cout << "| ";
        for (int i = 0; i < 8; i++)
        {
            // Check if file has not ended and char is not an endl.
            if (buffer[i] != 999 || buffer[i] != 10)
            {
                // Convert int to char and print.
                converted = (char)buffer[i];
                if (converted == 10) std::cout << "_";
                else std::cout << converted;
            }
            // If the file has ended or we find an invisible char, just print spaces.
            else std::cout << " ";
        }
        std::cout << " |" << std::endl;
    }

    // Generate bottom.
    // ------------------------------------------------------
    std::cout << "╚";                                           // Cornerpiece
    if (lineNums) std::cout << "══" << "══════" << "╩";         // Line numbers
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════════════════════" << "╩";}// Hex rows
    for (int i = 0; i < rows; i++) {
        std::cout << "══" << "════════";                        // Char rows
        if (i != (rows -1)) std::cout << "╩";}
    std::cout << "╝" << std::endl;                              // Cornerpiece and final line break

    // Cleanup time
    // ------------------------------------------------------
    file.close();
    return 0;
}
