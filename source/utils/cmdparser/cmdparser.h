#pragma once

#include "./../file/file.h"

#include <string>
#include <map>
#include <vector>

namespace utils
{
    class cmdparser
    {
    private:
        std::string error;
        utils::file& file;
        std::map<std::string, bool(utils::cmdparser::*)(std::vector<std::string, std::allocator<std::string>> *tokens)> commands;

        std::vector<std::string>* lexer(std::string cmd);

        bool onExit(std::vector<std::string>* tokens);
        bool onSave(std::vector<std::string>* tokens);
        bool onMove(std::vector<std::string>* tokens);

    public:
        cmdparser(utils::file& file);

        bool executeCmd(std::string& cmd);
        std::string& getError();
    };
}