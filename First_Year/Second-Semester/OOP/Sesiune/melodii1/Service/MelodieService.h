//
// Created by octav on 24.06.2026.
//

#ifndef MELODII1_MELODIESERVICE_H
#define MELODII1_MELODIESERVICE_H
#include "../repository/MelodieRepository.h"
#include "../domain/validator/ValidatorMelodie.h"


class MelodieService
{
private:
    MelodieRepository& repo;
    ValidatorMelodie& validator;

public:
    MelodieService(MelodieRepository& repository,ValidatorMelodie& validator) : repo(repository),validator(validator){}
    vector<Melodie> getAllSorted() const;

    const Melodie& find(int id) const;

    void update(int id,string title,int rank);

    void remove(int id);

    int countRank(int rank) const;

    vector<int> rankFrequency() const;
};


#endif //MELODII1_MELODIESERVICE_H