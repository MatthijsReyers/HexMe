#pragma once

#include "./../app.h"
#include "./../../utils/file/file.h"
#include "./../../exceptions/exceptions.h"

#include <sstream>
#include <string>
#include <map>
#include <vector>

class cmdparser
{
private:
    typedef void (cmdparser::*method)(std::vector<std::string>* tokens);

    utils::file& file;
    app* hexme;
    std::map<std::string,method> commands;

    std::vector<std::string>* lexer(std::string cmd);

    void onExit(std::vector<std::string>* tokens);
    void onGoto(std::vector<std::string>* tokens);
    void onInsert(std::vector<std::string>* tokens);
    void onReplace(std::vector<std::string>* tokens);
    void onMove(std::vector<std::string>* tokens);

public:
    cmdparser(utils::file& file, app* hexme);

    void executeCmd(std::string& cmd);
};

struct CmdSyntaxErrorException : public HexMeException
{
    public:
        CmdSyntaxErrorException(std::string msg) {this->message = msg;}
};
