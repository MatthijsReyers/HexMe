#include "cmdparser.h"

namespace utils
{
    cmdparser::cmdparser(utils::file& f): file(f)
    {
        this->commands["exit"] = &cmdparser::onExit;
    }

    std::vector<std::string>* cmdparser::lexer(std::string cmd)
    {
        auto res = new std::vector<std::string>();
        std::stringstream ss(cmd);
        std::string segment;
        while (std::getline(ss, segment, ' '))
            if (segment != "")
                res->push_back(segment);
        return res;
    }

    bool cmdparser::onExit(std::vector<std::string>* tokens)
    {
        exit(0);
    }

    bool cmdparser::executeCmd(std::string& cmd)
    {
        auto tokens = lexer(cmd);
        
        // A command is at least one token long.
        if (tokens->size() == 0) {
            this->error = "Please enter a command to execute.";
            delete tokens;
            return false;
        }

        // Check if given command exists.
        else if (commands.find((*tokens)[0]) != commands.end()) {
            return true;
        }

        // Comand does not exist.
        else {
            this->error = "Please enter a command to execute.";
            delete tokens;
            return false;
        }
    }
}