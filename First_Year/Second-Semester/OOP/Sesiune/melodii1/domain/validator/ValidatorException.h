//
// Created by octav on 24.06.2026.
//

#ifndef MELODII1_VALIDATOREXCEPTION_H
#define MELODII1_VALIDATOREXCEPTION_H
#include <exception>
#include <string>
#include <utility>

using std::string;

class ValidatorException : public std::exception
{
private:
    string msg;

public:
    ValidatorException(const string& msg) : msg(msg){};

    const string& getMessage() const
    {
        return msg;
    }
};


#endif //MELODII1_VALIDATOREXCEPTION_H