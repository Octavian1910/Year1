//
// Created by octav on 25.06.2026.
//

#include "Repository.h"

#include <fstream>
#include <string>
#include <sstream>

#include "RepositoryException.h"

using std::string;


Repository::Repository(string fileName)
{
    this->fileName = fileName;
    loadFromFile();
}

void Repository::loadFromFile()
{
    all.clear();

    std::ifstream fin(fileName);

    if (!fin.is_open())
    {
        throw RepositoryException("Nu s a putut deschide fisierul!");
    }

    string line;
    while (std::getline(fin,line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        string idStr,dimStr,tabla,jucatorStr,stare1,stare2,stare;
        std::getline(ss,idStr,' ');
        std::getline(ss,dimStr,' ');
        std::getline(ss,tabla,' ');
        std::getline(ss,jucatorStr,' ');
        std::getline(ss,stare1,' ');
        std::getline(ss,stare2,' ');
        if (!stare2.empty())
            stare = stare1+ ' ' +stare2;
        else stare = stare1;

        int id = std::stoi(idStr);
        int dim = std::stoi(dimStr);
        char jucator = jucatorStr[0];

        Joc joc{id,dim,tabla,jucator,stare};
        all.push_back(joc);

    }
    fin.close();
}

void Repository::storeToFile()
{
    std::ofstream fout(fileName);
    if (!fout.is_open())
    {
        throw RepositoryException("Nu s a putut deschide fisierul pentru scriere!");
    }

    for (const auto& joc : all)
    {
        fout << joc.getId() << " " << joc.getDim() << " " << joc.getTabla() << " " << joc.getJucatorCurent() << " " << joc.getStare() << "\n";
    }

    fout.close();
}

const std::vector<Joc> &Repository::getAll() const
{
    return all;
}

Joc Repository::findById(int id) const
{
    for (const auto& el : all)
    {
        if (el.getId() == id)
            return el;
    }
    throw RepositoryException("Nu exista joc cu id-ul dat!");
}

void Repository::add(const Joc &joc)
{
    bool existent = false;
    for (const auto& el:all)
    {
        if (el.getId() == joc.getId())
        {
            existent = true;
            break;
        }
    }

    if (!existent)
    {
        all.push_back(joc);
        storeToFile();
    }
    else
    {
        throw RepositoryException("Exista deja un joc cu acest id!");
    }


}

void Repository::update(const Joc &jocNou)
{
    bool gasit = false;
    for (auto& el : all)
    {
        if (el.getId() == jocNou.getId())
        {
            el.setDim(jocNou.getDim());
            el.setJucatorCurent(jocNou.getJucatorCurent());
            el.setStare(jocNou.getStare());
            el.setTabla(jocNou.getTabla());
            gasit = true;
            break;
        }
    }

    if (gasit == false)
    {
        throw RepositoryException("Nu exista joc cu id-ul dat!");
    }
    storeToFile();
}
