//
// Created by octav on 25.06.2026.
//

#include "Service.h"
#include "ServiceException.h"

using std::string;

/*
 * Folosim o prioritate pentru stari, ca sa putem sorta jocurile:
 * Neinceput < In derulare < Terminat
 */
static int prioritateStare(const std::string& stare)
{
    if (stare == "Neinceput")
        return 0;

    if (stare == "In derulare")
        return 1;

    return 2; // Terminat
}

/*
 * Schimba jucatorul curent:
 * X -> 0
 * 0 -> X
 */
static char schimbaJucator(char jucator)
{
    if (jucator == 'X')
        return '0';

    return 'X';
}

/*
 * Verifica daca pozitia este valida in tabla.
 * Pentru dimensiune dim, pozitiile valide sunt:
 * 0 ... dim*dim - 1
 */
static bool pozitieValida(int poz, int dim)
{
    return poz >= 0 && poz < dim * dim;
}

/*
 * Verifica daca tabla este plina.
 * Daca nu mai exista '-', inseamna ca jocul s-a terminat.
 */
static bool tablaPlina(const std::string& tabla)
{
    for (char ch : tabla)
    {
        if (ch == '-')
            return false;
    }
    return true;
}

/*
 * Returneaza toate jocurile exact cum sunt in repository.
 */
const std::vector<Joc>& Service::getAll() const
{
    return repo.getAll();
}

/*
 * Returneaza toate jocurile sortate dupa stare:
 * Neinceput -> In derulare -> Terminat
 *
 * Facem un bubble sort simplu, ca sa ramana in stilul tau.
 */
std::vector<Joc> Service::getAllSorted() const
{
    auto all = repo.getAll();

    for (int i = 0; i < (int)all.size(); i++)
    {
        for (int j = 0; j < (int)all.size() - 1; j++)
        {
            int stare1 = prioritateStare(all[j].getStare());
            int stare2 = prioritateStare(all[j + 1].getStare());

            if (stare1 > stare2)
            {
                std::swap(all[j], all[j + 1]);
            }
        }
    }

    return all;
}

/*
 * Adauga un joc nou.
 * 1) construim jocul
 * 2) il validam
 * 3) il adaugam in repo
 * 4) anuntam observerii (GUI-ul)
 */
void Service::addJoc(int id, int dim, const string& tabla, char jucatorCurent, const string& stare)
{
    Joc joc{id, dim, tabla, jucatorCurent, stare};
    val.validate(joc);
    repo.add(joc);
}

/*
 * Actualizeaza un joc existent.
 * 1) construim noul joc
 * 2) il validam
 * 3) facem update in repo
 * 4) anuntam observerii
 */
void Service::updateJoc(int id, int dim, const string& tabla, char jucatorCurent, const string& stare)
{
    Joc joc{id, dim, tabla, jucatorCurent, stare};
    val.validate(joc);
    repo.update(joc);
}

/*
 * Cauta jocul dupa id.
 */
Joc Service::findById(int id) const
{
    return repo.findById(id);
}

/*
 * Face o mutare in jocul cu id-ul dat, pe pozitia pozitie.
 *
 * Pasii:
 * 1) luam jocul din repo
 * 2) verificam daca pozitia e valida
 * 3) verificam daca celula este libera
 * 4) punem simbolul jucatorului curent
 * 5) daca tabla e plina -> jocul devine Terminat
 * 6) altfel jocul devine In derulare si schimbam jucatorul
 * 7) validam jocul
 * 8) facem update in repo
 * 9) notify() pentru GUI
 */
void Service::makeMove(int id, int pozitie)
{
    Joc joc = repo.findById(id);

    int dim = joc.getDim();
    string tabla = joc.getTabla();

    // verificam daca pozitia este valida
    if (!pozitieValida(pozitie, dim))
    {
        throw ServiceException("Pozitie invalida!");
    }

    // verificam daca pozitia este libera
    if (tabla[pozitie] != '-')
    {
        throw ServiceException("Pozitia nu e libera!");
    }

    // punem simbolul jucatorului curent pe tabla
    tabla[pozitie] = joc.getJucatorCurent();
    joc.setTabla(tabla);

    // daca dupa mutare tabla este plina, jocul se termina
    if (tablaPlina(tabla))
    {
        joc.setStare("Terminat");
    }
    else
    {
        // altfel jocul este in derulare
        joc.setStare("In derulare");

        // schimbam jucatorul doar daca jocul nu s-a terminat
        joc.setJucatorCurent(schimbaJucator(joc.getJucatorCurent()));
    }

    // validam jocul modificat
    val.validate(joc);

    // salvam modificarile in repository
    repo.update(joc);

    // anuntam toate ferestrele sa se actualizeze
}