//
// Created by octav on 24.06.2026.
//

#ifndef MELODII1_SERVICEEXCEPTION_H
#define MELODII1_SERVICEEXCEPTION_H
#include <exception>
#include <string>
#include <utility>
using std::exception;
using std::string;

class ServiceException : public exception
{
private:
    string msg;

public:
    ServiceException(string msg): msg(std::move(msg)){}
    const string& getMessage() const
    {
        return msg;
    }
};


#endif //MELODII1_SERVICEEXCEPTION_H