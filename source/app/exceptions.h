#pragma once

#include <string>
#include <iostream>
#include <exception>

struct HexMeException : public std::exception
{
    public:
        const std::string message = "";
        void printError() const {std::cout << "ERROR: " << message << std::endl;}
        HexMeException() {}
        HexMeException(const char* msg) : message(std::string(msg)) {}
        HexMeException(const std::string msg) : message(msg) {}
};

struct FailedToOpenFileException : public HexMeException
{
    public:
        std::string fileUrl;
        FailedToOpenFileException(const std::string msg, const std::string file) : HexMeException(msg), fileUrl(file) {}
        void printError() const {std::cout << "ERROR: the file \"" << fileUrl << "\" could not be opened, " << message << std::endl;}
};

struct CouldNotLoadSettingsException : public HexMeException
{
    public:
        std::string settingsFileUrl;
        CouldNotLoadSettingsException(std::string url) {this->settingsFileUrl = url;}
        void printError() const {std::cout << "ERROR: \"" << settingsFileUrl << "\" is not a " << std::endl;}
};

