//
// Created by octav on 24.06.2026.
//

#ifndef MELODII1_REPOSITORYEXCEPTION_H
#define MELODII1_REPOSITORYEXCEPTION_H
#include <exception>
#include <string>
using std::string;
class RepositoryException : std::exception
{
private:
    string msg;

public:
    RepositoryException(string msg) : msg(msg){}
    const string& getMessage() const
    {
        return msg;
    };
};


#endif //MELODII1_REPOSITORYEXCEPTION_H