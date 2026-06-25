//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_VALIDATOREXCEPTION_H
#define TICTACTOE_VALIDATOREXCEPTION_H
#include <string>
#include <utility>
using std::string;

class ValidatorException
{
private:
    string msg;

public:
    ValidatorException(string  msg) : msg(std::move(msg)) {}
    string getMessage() const
    {
        return msg;
    }


};


#endif //TICTACTOE_VALIDATOREXCEPTION_H