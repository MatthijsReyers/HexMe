#pragma once

#include <string>
#include <iostream>
#include <exception>

struct HexMeException : public std::exception
{
    public:
        std::string message = "";
        HexMeException() {}
        HexMeException(const char* msg) : message(std::string(msg)) {}
        HexMeException(const std::string msg) : message(msg) {}
};

struct WindowTooSmallException : HexMeException
{
    public:
        WindowTooSmallException() {
            message = "This terminal is too small to display the HexMe interface";
        }
};