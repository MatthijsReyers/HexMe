#include "cmdparser.h"

#include <curses.h>

cmdparser::cmdparser(utils::file& f, app* h): file(f), hexme(h)
{
    this->commands["exit"] = &cmdparser::onExit;
    this->commands["save"] = &cmdparser::onSave;
    this->commands["insert"] = &cmdparser::onInsert;
    this->commands["replace"] = &cmdparser::onReplace;
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
    hexme->close();
    exit(0);
    return false;
}

bool cmdparser::onSave(std::vector<std::string>* tokens)
{
    try {
        file.save();
        return true;
    }
    catch(const std::exception& e) {
        return false;
    }
}

bool cmdparser::onInsert(std::vector<std::string>* tokens)
{
    // Something must be given after replace.
    if (tokens->size() < 2) {
        this->error = "Please give bytes to insert.";
        return false;
    }
    
    // Do some magic with the tokens.
    const char* toInsert = (*tokens)[1].c_str();
    const int length = (*tokens)[1].length();

    // Replace bytes at cursor.
    file.insertBytes(toInsert, length);
    return true;
}

bool cmdparser::onReplace(std::vector<std::string>* tokens)
{
    // Something must be given after replace.
    if (tokens->size() < 2) {
        this->error = "Please give bytes to replace the bytes at cursor with.";
        return false;
    }
    
    // Do some magic with the tokens.
    const char* newBytes = (*tokens)[1].c_str();
    const int length = (*tokens)[1].length();

    // Replace bytes at cursor.
    file.replaceBytes(newBytes, length);
    return true;
}

bool cmdparser::executeCmd(std::string& cmd)
{
    // Spit command up into tokens.
    auto tokens = lexer(cmd);
    
    // A command is at least one token long.
    if (tokens->size() == 0) {
        this->error = "Please enter a command to execute.";
        delete tokens;
        return false;
    }

    // Check if given command exists.
    else if (commands.find((*tokens)[0]) != commands.end()) {
        std::string name = (*tokens)[0];
        method command = commands[name];
        return (this->*command)(tokens);
    }

    // Comand does not exist.
    else {
        mvaddstr(0,0,"That command does not exist.");
        this->error = "That command does not exist.";
        delete tokens;
        return false;
    }
}