#pragma once

#include <string>
#include <iostream>
#include <exception>

struct HexMeException : public std::exception
{
    public:
        std::string message;
        void printError() {std::cout << "ERROR: " << message << std::endl;}
        HexMeException() {this->message = "";}
        HexMeException(const char* msg) {this->message = std::string(msg);}
        HexMeException(std::string msg) {this->message = msg;}
};

struct InvalidArgsException : public HexMeException
{
    public:
        InvalidArgsException(std::string msg) {this->message = msg;}
};

struct FailedToOpenFileException : public HexMeException
{
    public:
        std::string fileUrl;
        FailedToOpenFileException(std::string msg, std::string file) {this->message = msg; this->fileUrl = file;}
        void printError() {std::cout << "ERROR: the file \"" << fileUrl << "\" could not be opened, " << message << std::endl;}
};

struct CouldNotLoadSettingsException : public HexMeException
{
    public:
        std::string settingsFileUrl;
        CouldNotLoadSettingsException(std::string url) {this->settingsFileUrl = url;}
        void printError() {std::cout << "ERROR: \"" << settingsFileUrl << "\" is not a " << std::endl;}
};

