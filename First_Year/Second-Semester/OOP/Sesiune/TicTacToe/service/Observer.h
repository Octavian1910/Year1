//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_OBSERVER_H
#define TICTACTOE_OBSERVER_H


class Observer
{
public:
    virtual void update() = 0;
    virtual ~Observer() = default;
};


#endif //TICTACTOE_OBSERVER_H