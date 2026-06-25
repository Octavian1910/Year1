//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_SERVICEEXCEPTION_H
#define TICTACTOE_SERVICEEXCEPTION_H
#include <exception>
#include <string>
using std::string;

class ServiceException : public std::exception
{
private:
    string msg;
public:
    ServiceException(string msg) : msg(std::move(msg)) {}
    string getMessage() const
    {
        return msg;
    }


};


#endif //TICTACTOE_SERVICEEXCEPTION_H