
// 18-05-2019 Matthijs Reyers
// 
// Mingw64 compiler args:
// -std=gnu++11
// 
// g++ -pthread -o hexme main.cpp 

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <stdio.h>

// Main vars
std::ifstream file;


int main(int argv, char** argc)
{
    // Extract filename from commandline args.
    // ------------------------------------------------------
    file.open("testfile.hex");
    if (!file)
    {
        printf("There was a problem opening the file.\n");
        exit(1);
    }
    std::cout << "--------------------------------------" << std::endl;
    int buffer[8];
    char converted;
    while (file.good())
    {
        std::cout << "| ";
        for (int i = 0; i < 8; i++)
        {
            if (file.good()) buffer[i] = file.get();
            else buffer[i] = 999;
        }
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
    file.close();
}
