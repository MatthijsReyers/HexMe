#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

struct InvalidArgsException
{
    public:
        const std::string message = "";
        
        InvalidArgsException() {}
        InvalidArgsException(const char* msg) : message(std::string(msg)) {}
        InvalidArgsException(const std::string msg) : message(msg) {}
};

struct MissingArgsException
{
    public:
        const std::string message = "";
        
        MissingArgsException(const char* msg) : message(std::string(msg)) {}
        MissingArgsException(const std::string msg) : message(msg) {}

        static MissingArgsException from_switch(const std::string s) {
            std::stringstream ss;
            ss << "Missing required argument \"" << s << "\", please provide a value";
            return MissingArgsException(ss.str());
        }
};
