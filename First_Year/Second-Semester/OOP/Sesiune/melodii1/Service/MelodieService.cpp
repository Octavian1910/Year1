//
// Created by octav on 24.06.2026.
//

#include "MelodieService.h"

#include <algorithm>

#include "ServiceException.h"

vector<Melodie> MelodieService::getAllSorted() const
{
    vector<Melodie> rez= repo.getAll();
    std::sort(rez.begin(),rez.end(),
        [](const Melodie& m1,const Melodie& m2)
        {
            return m1.get_rank() < m2.get_rank();
        });

    return rez;
}


const Melodie &MelodieService::find(int id) const
{
    return repo.find(id);
}

void MelodieService::remove(int id)
{
    const Melodie& mel = repo.find(id);
    string artist = mel.get_artist();

    int cnt = 0;
    for (const auto& m : repo.getAll())
    {
        if (m.get_artist() == artist)
            cnt++;
    }

    if (cnt == 1)
        throw ServiceException("This is the last song of the artist and cannot be deleted!\n");

    repo.remove(id);
}

int MelodieService::countRank(int rank) const
{
    int cnt = 0;
    for (const auto& mel : repo.getAll())
    {
        if (mel.get_rank() == rank)
            cnt++;
    }
    return cnt;
}

vector<int> MelodieService::rankFrequency() const
{
    vector<int> freq(11,0);
    for (const auto& mel : repo.getAll())
    {
        freq[mel.get_rank()]++;
    }
    return freq;
}

void MelodieService::update(int id, string title, int rank)
{
    const Melodie& oldMel = repo.find(id);
    Melodie updated{id,title,oldMel.get_artist(),rank};
    ValidatorMelodie::validate(updated);
    repo.update(updated);
}



