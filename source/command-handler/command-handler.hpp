#pragma once

#include "./../app/app.h"
#include "./../app/exceptions.h"
#include "./../utils/file/file.h"
#include "./../utils/stringtools/escape.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>

/**
 * This class is responsible for parsing and executing the commands entered by the user in the 
 * command text-box.
 */
class CommandHandler
{
private:
    typedef void (CommandHandler::*cmdHandler)(std::vector<std::string>& tokens);

    utils::file& file;
    app* hexme;
    std::map<std::string, cmdHandler> commands;

    std::vector<std::string> lexer(std::string cmd);

    void onExit(std::vector<std::string>& tokens);
    void onOpen(std::vector<std::string>& tokens);
    void onGoto(std::vector<std::string>& tokens);
    void onFind(std::vector<std::string>& tokens);
    void onInsert(std::vector<std::string>& tokens);
    void onReplace(std::vector<std::string>& tokens);
    void onMove(std::vector<std::string>& tokens);

public:
    CommandHandler(utils::file& file, app* hexme);

    void executeCmd(std::string& cmd);
};

struct CmdSyntaxErrorException : public HexMeException
{
    public:
        CmdSyntaxErrorException(std::string msg) : HexMeException(msg) {}
};
