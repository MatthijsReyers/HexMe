#include "cmdparser.h"

#include <curses.h>

cmdparser::cmdparser(utils::file& f, app* h): file(f), hexme(h)
{
    this->commands["exit"] = &cmdparser::onExit;
    this->commands["goto"] = &cmdparser::onGoto;
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

void cmdparser::onExit(std::vector<std::string>* tokens)
{
    hexme->close();
    exit(0);
}

void cmdparser::onGoto(std::vector<std::string>* tokens)
{
    // Something must be given after 'goto'.
    if (tokens->size() < 2)
        throw CmdSyntaxErrorException("Please give a location to goto.");

    // Get first argument of command.
    auto type = (*tokens)[1];
    
    // File start and end shortcuts.
    if (type == "start")
        file.moveCursor(0);
    else if (type == "end")
        file.moveCursor(file.getFileEnd());

    // Hex or decimal notation.
    else if (type == "hex" || type == "dec") {

        // Catch syntax errors.
        if (tokens->size() < 3)
            throw CmdSyntaxErrorException("Please give a number after hex/dec.");
        else if (tokens->size() > 3)
            throw CmdSyntaxErrorException("Please give only one number after hex/dec.");

        // Try to convert string to int.
        try {
            unsigned long long cursor;
            if (type == "dec") cursor = std::stoi((*tokens)[2], nullptr, 10);
            else /*type == hex*/ cursor = std::stoi((*tokens)[2], nullptr, 16);
            
            if (cursor > file.getFileEnd())
                file.moveCursor(file.getFileEnd());
            else file.moveCursor(cursor);
        }
        catch (std::invalid_argument const &e)
        {
            throw CmdSyntaxErrorException("Number was not in the correct format.");
        }
    }

    // Try to determine notation if none is given.
    else {
        // Hex notation is concidered as default.
        try {
            unsigned long long cursor = std::stoi((*tokens)[2], nullptr, 16);
            file.moveCursor(cursor);
        }
        catch (std::invalid_argument const &e)
        {
            throw CmdSyntaxErrorException("Please specify number format.");
        }
    }
}

void cmdparser::onInsert(std::vector<std::string>* tokens)
{
    // Something must be given after replace.
    if (tokens->size() < 2)
        throw CmdSyntaxErrorException("Please give bytes to insert.");
    
    // Do some magic with the tokens.
    const char* toInsert = (*tokens)[1].c_str();
    const int length = (*tokens)[1].length();

    // Replace bytes at cursor.
    file.insertBytes(toInsert, length);
}

void cmdparser::onReplace(std::vector<std::string>* tokens)
{
    // Something must be given after replace.
    if (tokens->size() < 2)
        throw CmdSyntaxErrorException("Please give bytes to replace the bytes at cursor with.");

    else {
        // Do some magic with the tokens.
        const char* newBytes = (*tokens)[1].c_str();
        const int length = (*tokens)[1].length();

        // Replace bytes at cursor.
        file.replaceBytes(newBytes, length);
    }
}

void cmdparser::executeCmd(std::string& cmd)
{
    // Spit command up into tokens.
    auto tokens = lexer(cmd);
    
    // A command is at least one token long.
    if (tokens->size() == 0)
        throw CmdSyntaxErrorException("Please enter a command to execute.");

    // Check if given command exists.
    else if (commands.find((*tokens)[0]) != commands.end()) {
        std::string name = (*tokens)[0];
        method command = commands[name];
        (this->*command)(tokens);
    }

    // Comand does not exist.
    else throw CmdSyntaxErrorException("That command does not exist.");
}
