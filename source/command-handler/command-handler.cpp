#include "command-handler.hpp"

#include "./../utils/stringtools/escape.h"
#include "./../gui/message-box/message-box-okay.hpp"

#include <regex>
#include <sstream>

CommandHandler::CommandHandler(std::weak_ptr<File> f, HexMeApp *h) : file(f), app(h)
{
    this->commands["save"] = &CommandHandler::onSave;
    this->commands["echo"] = &CommandHandler::onEcho;
    this->commands["help"] = &CommandHandler::onHelp;
    this->commands["exit"] = &CommandHandler::onExit;
    this->commands["load"] = &CommandHandler::onOpen;
    this->commands["goto"] = &CommandHandler::onGoto;
    this->commands["move"] = &CommandHandler::onMove;
    this->commands["find"] = &CommandHandler::onFind;
    this->commands["insert"] = &CommandHandler::onInsert;
    this->commands["replace"] = &CommandHandler::onReplace;
    this->commands["delete"] = &CommandHandler::onDelete;
}

int64_t CommandHandler::parseInt(std::string &val)
{
    std::regex number_check(
        "^(\\-)?((0x[\\dA-fa-f]+)|(0b[0-1]+)|(\\d+))$",
        std::regex_constants::ECMAScript);

    if (!std::regex_search(val, number_check))
    {
        std::stringstream ss;
        ss << "Expression \"" << val << "\" is not a valid integer.";
        throw CmdSyntaxErrorException(ss.str());
    }

    auto base = 10;

    auto pos = val.find("0x");
    if (pos != std::string::npos)
    {
        base = 16;
        val.erase(pos, 2);
    }

    pos = val.find("0b");
    if (pos != std::string::npos)
    {
        base = 2;
        val.erase(pos, 2);
    }

    return std::stol(val, nullptr, base);
}

std::vector<std::string> CommandHandler::lexer(std::string cmd)
{
    // Vector to output.
    auto res = std::vector<std::string>();

    // Remove trailing spaces behind cmd string.
    while (cmd.back() == ' ')
        cmd.pop_back();

    // Parse keywords.
    std::stringstream ss(cmd);
    std::string segment;
    while (std::getline(ss, segment, ' '))
    {
        // If the segment is a string. (<= Surrounded by "".)
        if (segment.length() > 0 && segment[0] == '"')
        {

            // String must also have closing bracket/quote.
            if (cmd[cmd.size() - 1] != '"')
                /* Throw syntax error */
                throw std::exception();

            // Get full string.
            segment = cmd.substr(cmd.find(segment) + 1, cmd.size() - 1);
            segment.pop_back();

            // Take care of escaped characters in string.
            segment = utils::stringtools::escape(segment);

            // Push to output and break while loop.
            res.push_back(segment);
            break;
        }

        // Output vector may not include empty items.
        if (segment != "")
            res.push_back(segment);
    }

    return res;
}

void CommandHandler::onEcho(std::vector<std::string> &tokens)
{
    std::vector<std::string> msg;
    if (tokens.size() > 1) { msg.push_back(tokens[1]); }
    else { msg.push_back("[empty string]"); }
    logMessage("creating message box");
    auto msgBox = gui::MessageBoxOkay(this->app, msg);
    msgBox.display();
}

void CommandHandler::onExit(std::vector<std::string> &tokens)
{
    app->close();
    exit(0);
}

void CommandHandler::onSave(std::vector<std::string> &tokens)
{
    if (tokens.size() > 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: save [path]");

    auto f = this->file.lock();

    if (tokens.size() == 2) {
        auto path = tokens[1];
        f->save(path);
    } else {
        f->save();
    }
}

void CommandHandler::onHelp(std::vector<std::string> &tokens)
{
    std::vector<std::string> msg = {
        "exit                         - Closes application",
        "load [url]                   - Opens the given file",
        "goto (start/end/[int])       - Set cursor pos to index",
        "move [int]                   - Moves cursor relative",
        "find (first/next/last) [str] - Search for bytes",
        "insert [str]                 - Insert bytes at cursor",
        "replace [str]                - Overwrite bytes at cursor",
        "delete [int]                 - Remove bytes at cursor",
    };
    auto msgBox = gui::MessageBoxOkay(this->app, msg);
    msgBox.display();
}

void CommandHandler::onOpen(std::vector<std::string> &tokens)
{
    if (tokens.size() == 1)
        throw CmdSyntaxErrorException("Please give a location to go to.");
    if (tokens.size() > 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: load \"path/to/file.txt\"");

    auto f = this->file.lock();

    auto newPath = tokens[1];
    auto oldPath = f->getPath();

    try {
        f->load(newPath);
    }

    catch (FailedToOpenFileException &error)
    {
        f->load(oldPath);
        throw CmdSyntaxErrorException("Could not open file.");
    }
}

void CommandHandler::onGoto(std::vector<std::string> &tokens)
{
    if (tokens.size() == 1)
        throw CmdSyntaxErrorException("Please give an index to go to: goto (start|end|[int])");
    else if (tokens.size() > 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: goto (start|end|[int])");

    auto f = this->file.lock();

    if (tokens[1] == "start")
    {
        f->setCursor(0);
        return;
    }
    if (tokens[1] == "end")
    {
        f->setCursor(f->size());
        return;
    }

    try {
        // Convert string to unsigned long long.
        auto location = this->parseInt(tokens[1]);

        // Location must be inside file.
        if (location > f->size())
            throw CmdSyntaxErrorException("Provided index is located outside of file.");

        // Move cursor.
        else f->setCursor(location);
    }

    catch (std::invalid_argument const &e)
    {
        throw CmdSyntaxErrorException("Number is not in the correct format.");
    }
    catch (std::out_of_range const &e)
    {
        throw CmdSyntaxErrorException("Provided number is too large.");
    }
}

void CommandHandler::onMove(std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: move [number].");

    try
    {
        auto f = this->file.lock();

        int64_t direction = this->parseInt(tokens[1]);
        int64_t current = f->getCursor();
        int64_t end = f->size();
        int64_t position = std::clamp(current + direction, int64_t{0}, end);

        f->setCursor(position);
    }
    catch (std::out_of_range const &e)
    {
        throw CmdSyntaxErrorException("Provided number is too large.");
    }
}

void CommandHandler::onFind(std::vector<std::string> &tokens)
{
    if (tokens.size() == 1)
        throw CmdSyntaxErrorException("Please give a string to find.");
    if (tokens.size() > 3)
        throw CmdSyntaxErrorException("Please use the correct syntax: find [first/last/next] \"query\"");

    std::string query;
    unsigned long long start, stop;
    auto f = file.lock();
    auto cursor = f->getCursor();

    // Find first occurrence of string.
    if (tokens[1] == "first" || tokens.size() == 2)
    {
        if (tokens.size() == 2) query = tokens[1];
        else query = tokens[2];
        start = 0;
        stop = f->size();
    }

    // Find last occurrence of string.
    else if (tokens[1] == "last")
    {
        query = tokens[2];
        start = f->size();
        stop = 0;
    }

    // Find previous occurrence of string.
    else if (tokens[1] == "previous" || tokens[1] == "prev")
    {
        query = tokens[2];
        start = f->getCursor();
        stop = 0;
    }

    // Find next occurrence of string.
    else if (tokens[1] == "next")
    {
        query = tokens[2];
        start = f->getCursor() + 1;
        stop = f->size() + 1;
    }

    // Wrong syntax.
    else throw CmdSyntaxErrorException("Please use the correct syntax: find [first/prev/last/next] \"query\"");

    // Search for string.
    for (unsigned long long i = start; i != stop; (start < stop) ? i++ : i--)
    {
        f->setCursor(i);
        u_int8_t current = f->getCurrentByte();
        if (current == query[0] && f->getBytesAfterCursor() >= query.size())
        {
            bool found = true;
            for (unsigned long long a = 0; a < query.size() && found; a++)
            {
                f->setCursor(i + a);
                if (query[a] != f->getCurrentByte())
                    found = false;
            }

            // Move cursor to query location and find.
            if (found)
            {
                f->setCursor(i);
                return;
            }
        }
    }

    // Could not find query.
    f->setCursor(cursor);
    throw CmdSyntaxErrorException("Could not find provided query.");
}

void CommandHandler::onInsert(std::vector<std::string> &tokens)
{
    if (tokens.size() == 1)
        throw CmdSyntaxErrorException("Please give a string to insert.");
    if (tokens.size() > 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: insert \"string\".");

    const u_int8_t* token = reinterpret_cast<const u_int8_t*>(tokens[1].c_str());
    std::vector<u_int8_t> bytes;
    bytes.assign(token, token + tokens[1].length());
    this->file.lock()->insertBytes(bytes);
}

void CommandHandler::onReplace(std::vector<std::string> &tokens)
{
    if (tokens.size() == 1)
        throw CmdSyntaxErrorException("Please give bytes to replace the bytes at cursor with.");
    if (tokens.size() > 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: replace \"string\".");

    const u_int8_t* token = reinterpret_cast<const u_int8_t*>(tokens[1].c_str());
    std::vector<u_int8_t> bytes;
    bytes.assign(token, token + tokens[1].length());
    this->file.lock()->overwriteBytes(bytes);
}

void CommandHandler::onDelete(std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
        throw CmdSyntaxErrorException("Please use the correct syntax: delete [number].");
    
    try {
        auto f = this->file.lock();
        int64_t amount = this->parseInt(tokens[1]);
        f->deleteBytes(amount);
    }
    catch (std::out_of_range const &e)
    {
        throw CmdSyntaxErrorException("Provided number is too large.");
    }
}

void CommandHandler::executeCmd(std::string &cmd)
{
    // Spit command up into tokens.
    auto tokens = lexer(cmd);

    // A command is at least one token long.
    if (tokens.size() == 0) return;

    // Check if given command exists.
    else if (commands.find(tokens[0]) != commands.end())
    {
        std::string name = tokens[0];
        cmdHandler command = commands[name];
        (this->*command)(tokens);
    }

    else throw CmdSyntaxErrorException("That command does not exist.");
}
