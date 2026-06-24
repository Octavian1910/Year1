//
// Created by octav on 24.06.2026.
//

#ifndef MELODII1_MELODIEREPOSITORY_H
#define MELODII1_MELODIEREPOSITORY_H
#include <vector>
#include <string>
#include "../domain/Melodie.h"
using std::vector;
using std::string;

class MelodieRepository
{
private:
    vector<Melodie> melodii;
    string fileName;

    void loadFromFile();
    void storeToFile() const;

public:
    MelodieRepository(const string& fileName);
    const Melodie& find(int id) const;
    void update(const Melodie& melodie);
    void remove(int id);
    const vector<Melodie>& getAll() const;
    int size() const;

};


#endif //MELODII1_MELODIEREPOSITORY_H