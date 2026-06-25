//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_REPOSITORY_H
#define TICTACTOE_REPOSITORY_H
#include <vector>
#include <string>
#include "../domain/Joc.h"


class Repository
{
private:
    std::vector<Joc> all;
    string fileName;

    void loadFromFile();
    void storeToFile();

public:
    Repository(string fileName);
    const std::vector<Joc>& getAll() const;
    void add(const Joc& joc);
    void update(const Joc& jocNou);
    Joc findById(int id) const;
};


#endif //TICTACTOE_REPOSITORY_H