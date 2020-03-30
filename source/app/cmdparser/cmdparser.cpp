#include "cmdparser.h"
#include <unistd.h>


cmdparser::cmdparser(utils::file& f, app* h): file(f), hexme(h)
{
    this->commands["exit"] = &cmdparser::onExit;
    this->commands["goto"] = &cmdparser::onGoto;
    this->commands["find"] = &cmdparser::onFind;
    this->commands["insert"] = &cmdparser::onInsert;
    this->commands["replace"] = &cmdparser::onReplace;
}

std::vector<std::string>* cmdparser::lexer(std::string cmd)
{
    // Vector to output.
    auto res = new std::vector<std::string>();

    // Remove trailing spaces behind cmd string.
    while (cmd.back() == ' ')
        cmd.pop_back();

    // Parse keywords.
    std::stringstream ss(cmd);
    std::string segment;
    while (std::getline(ss, segment, ' '))
    {
        // If the segment is a string. (<= Surrounded by "".)
        if (segment.length() > 0 && segment[0] == '"') {

            // String must also have closing bracket/quote.
            if (cmd[cmd.size()-1] != '"')
                /* Throw syntax error */
                throw std::exception();

            // Get full string.
            segment = cmd.substr(cmd.find(segment)+1, cmd.size()-1);
            segment.pop_back();

            // Take care of escaped characters in string.
            segment = utils::stringtools::escape(segment);

            // Push to output and break while loop.
            res->push_back(segment);
            break;
        }

        // Output vector may not include empty items.
        if (segment != "")
            res->push_back(segment);
    }
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
    if (tokens->size() == 1)
        throw CmdSyntaxErrorException("Please give a location to go to.");

    else if (tokens->size() == 2)
    {
        // Get first argument of command.
        auto type = (*tokens)[1];
        
        // File start and end shortcuts.
        if (type == "start")
            file.moveCursor(0);
        else if (type == "end")
            file.moveCursor(file.getFileEnd());

        // Interpret first argument as hex number.
        else try {file.moveCursor(std::stoi(type, nullptr, 16));}
        catch (std::invalid_argument const &e) {
            throw CmdSyntaxErrorException("Number was not in the correct format.");
        }
    }

    // Hex or decimal notation.
    else if (tokens->size() == 3)
    {
        auto format = (*tokens)[1];
        auto num = (*tokens)[2];
        
        // Interpret second argument as hex number.
        if (format == "hex")
            try {file.moveCursor(std::stoi(num, nullptr, 16));}
            catch (std::invalid_argument const &e) {
                throw CmdSyntaxErrorException("Number was not in the specified format.");}
        
        // Interpret second argument as decimal number.
        else if (format == "dec")
            try {file.moveCursor(std::stoi(num, nullptr, 16));}
            catch (std::invalid_argument const &e) {
                throw CmdSyntaxErrorException("Number was not in the specified format.");}

        else throw CmdSyntaxErrorException("Please give a number after hex/dec.");
    }

    else throw CmdSyntaxErrorException("Please use the correct syntax.");
}

void cmdparser::onFind(std::vector<std::string>* tokens)
{
    if (tokens->size() == 1)
        throw CmdSyntaxErrorException("Please give a string to find.");
    if (tokens->size() > 3)
        throw CmdSyntaxErrorException("Please use the correct syntax: find [first/last/next] \"query\"");
    
    std::string query;
    unsigned long long start, stop;
    auto cursor = file.getCursorLocation();

    // Find first occurence of string.
    if ((*tokens)[1] == "first" || tokens->size() == 2)
    {
        if (tokens->size() == 2) query = (*tokens)[1];
        else query = (*tokens)[2];
        start = 0;
        stop = file.getFileEnd();
    }

    // Find last occurence of string.
    else if ((*tokens)[1] == "last")
    {
        query = (*tokens)[2];
        start = file.getFileEnd();
        stop = 0;
    }

    // Find next occurence of string.
    else if ((*tokens)[1] == "next")
    {
        query = (*tokens)[2];
        start = file.getCursorLocation() + 1;
        stop = file.getFileEnd() + 1;
    }

    // Wrong syntax.
    else throw CmdSyntaxErrorException("Please use the correct syntax: find [first/last/next] \"query\"");

    // Search for query.
    for (unsigned long long i = start; i != stop; (start < stop) ? i++ : i--)
    {
        file.moveCursor(i);
        byte current = file.getCurrentByte();
        if (current == query[0] && file.getBytesAfterCursor()+1 >= query.size())
        {
            bool found = true;
            for (unsigned long long a = 0; a < query.size() && found; a++)
            {
                file.moveCursor(i + a);
                if (query[a] != file.getCurrentByte())
                    found = false;
            }

            // Move cursor to query location and find.
            if (found) {
                file.moveCursor(i);
                return;
            }
        }
    }
    
    // Could not find query.
    file.moveCursor(cursor);
    throw CmdSyntaxErrorException("Could not find provided query");
}

void cmdparser::onInsert(std::vector<std::string>* tokens)
{
    if (tokens->size() == 1)
        throw CmdSyntaxErrorException("Please give a string to insert.");
    if (tokens->size() > 3)
        throw CmdSyntaxErrorException("Please use the correct syntax.");
    
    // Do some magic with the tokens.
    const char* toInsert = (*tokens)[1].c_str();
    const int length = (*tokens)[1].length();

    // Replace bytes at cursor.
    file.insertBytes(toInsert, length);
}

void cmdparser::onReplace(std::vector<std::string>* tokens)
{
    // Something must be given after replace.
    if (tokens->size() == 1)
        throw CmdSyntaxErrorException("Please give bytes to replace the bytes at cursor with.");

    else if (tokens->size() == 2)
    {
        // Do some magic with the tokens.
        const char* newBytes = (*tokens)[1].c_str();
        const int length = (*tokens)[1].length();

        // Replace bytes at cursor.
        file.replaceBytes(newBytes, length);
    }

    else throw CmdSyntaxErrorException("Please use the correct syntax.");
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
        delete tokens;
    }

    // Comand does not exist.
    else throw CmdSyntaxErrorException("That command does not exist.");
}
