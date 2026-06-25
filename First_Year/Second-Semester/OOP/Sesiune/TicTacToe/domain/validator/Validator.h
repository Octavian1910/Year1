//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_VALIDATOR_H
#define TICTACTOE_VALIDATOR_H
#include "../domain/Joc.h"
#include <exception>


class Validator : public std::exception
{
public:
    void validate(const Joc& joc) const;
};


#endif //TICTACTOE_VALIDATOR_H