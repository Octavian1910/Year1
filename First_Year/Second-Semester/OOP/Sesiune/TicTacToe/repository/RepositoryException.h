//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_REPOSITORYEXCEPTION_H
#define TICTACTOE_REPOSITORYEXCEPTION_H
#include <string>
#include <exception>
using std::string;

class RepositoryException : public std::exception
{
private:
    string msg;

public:
    RepositoryException(string msg) : msg(std::move(msg)){}
    string getMessage() const
    {
        return msg;
    }


};


#endif //TICTACTOE_REPOSITORYEXCEPTION_H