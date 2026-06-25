//
// Created by octav on 25.06.2026.
//

#include "Validator.h"

#include <csignal>
#include <exception>

#include "../domain/Joc.h"

#include <string>


#include "ValidatorException.h"
using std::string;
using std::exception;

static bool dimValida(int dim)
{
    return dim == 3 || dim == 4 || dim == 5;
}

static bool stareValida(const std::string& stare)
{
    return stare == "Neinceput" || stare == "In derulare" || stare == "Terminat";
}

static bool jucatorValid(char jucator)
{
    return jucator == 'X' || jucator == '0';
}

static bool caractereOK(const string& tabla)
{
    for (auto ch : tabla)
    {
        if ( ch != 'X' && ch != '0' && ch != '-')
        {
            return false;
        }
    }
    return true;
}

void Validator::validate(const Joc &joc) const
{
    string errors;
    if (!dimValida(joc.getDim()))
    {
        errors += "Dimensiunea trebuie sa fie 3 ,4 sau 5!\n";
    }

    int dim = joc.getDim();
    const string& tabla = joc.getTabla();
    if ((int)tabla.size() != dim*dim )
    {
        errors += "Tabla trebuie sa aiba exact dim*dim caractere!\n";
    }
    if (!caractereOK(tabla))
    {
        errors += "Tabla poate contine doar 0 X sau - !\n";
    }

    if (!jucatorValid(joc.getJucatorCurent()))
    {
        errors += "Jucatorul curent poate sa fie  X sau 0!\n";
    }

    if (!stareValida(joc.getStare()))
    {
        errors += "Starea trebuie sa fie Neinceput , In derulare sau Terminat!\n";
    }

    if (!errors.empty())
    {
        throw ValidatorException(errors);
    }

}
