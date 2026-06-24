//
// Created by octav on 24.06.2026.
//

#include "MelodieRepository.h"

#include <algorithm>

#include "../domain/Melodie.h"
#include <fstream>
#include <sstream>

#include "RepositoryException.h"

MelodieRepository::MelodieRepository(const string &fileName)
{
    this->fileName = fileName;
    loadFromFile();
}

void MelodieRepository::loadFromFile()
{
    melodii.clear();

    std::ifstream in(fileName);
    if (!in.is_open())
    {
        throw RepositoryException("File could not be opened!\n");
    }

    string line;
    while (std::getline(in,line))
    {
        if (line.empty())
            continue;

        std::stringstream linestream(line);
        string id_str,title,artist,rank_str;
        getline(linestream,id_str,',');
        getline(linestream,title,',');
        getline(linestream,artist,',');
        getline(linestream,rank_str,',');

        int id = std::stoi(id_str);
        int rank = std::stoi(rank_str);
        Melodie m{id,title,artist,rank};
        melodii.push_back(m);
    }
    in.close();
}

void MelodieRepository::storeToFile() const
{
    std::ofstream out(fileName);
    if (!out.is_open())
    {
        throw RepositoryException("File could not be opened!\n");
    }

    for (const auto& m : melodii)
    {
        out << m.get_ID() << "," << m.get_title() << "," << m.get_artist() << "," << m.get_rank() << "\n";
    }
    out.close();
}


const Melodie &MelodieRepository::find(int id) const
{
    auto it = std::find_if(melodii.begin(),melodii.end(),
        [id](const Melodie& m)
        {
           return m.get_ID() == id;
        });

    if (it == melodii.end())
        throw RepositoryException("Song does not exist!\n");

    return *it;
}

void MelodieRepository::update(const Melodie &melodie)
{
    auto it = std::find_if(melodii.begin(),melodii.end(),
        [&melodie](const Melodie& m)
        {
           return m.get_ID() == melodie.get_ID();
        });

    if (it == melodii.end())
        throw RepositoryException("Song does not exist and cannot be updated!\n");

    it->set_artist(melodie.get_artist());
    it->set_rank(melodie.get_rank());
    it->set_title(melodie.get_title());

    storeToFile();
}

const vector<Melodie> &MelodieRepository::getAll() const
{
    return melodii;
}

void MelodieRepository::remove(int id)
{
    auto it = std::find_if(melodii.begin(),melodii.end(),
        [id](const Melodie& m)
        {
           return m.get_ID() == id;
        });

    if ( it == melodii.end())
        throw RepositoryException("Song does not exist and cannot be removed!\n");

    melodii.erase(it);
    storeToFile();
}

