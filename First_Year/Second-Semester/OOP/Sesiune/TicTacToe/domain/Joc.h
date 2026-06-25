//
// Created by octav on 25.06.2026.
//

#ifndef TICTACTOE_JOC_H
#define TICTACTOE_JOC_H
#include <string>
#include <utility>
using std::string;

class Joc
{
private:
    int id;
    int dim;
    string tabla;
    char jucatorCurent;
    string stare;


public:
    Joc() = default; //il lasam asa ca sa putem crea obiecte si fara parametrii
    Joc(int id,int dim , string tabla,char jucatorCurent,string stare) : id(id),dim(dim),tabla(std::move(tabla)),jucatorCurent(jucatorCurent),stare(std::move(stare)) {}

    int getId() const
    {
        return id;
    }

    int getDim() const
    {
        return dim;
    }

    const string& getTabla() const
    {
        return tabla;
    }

    char getJucatorCurent() const
    {
        return jucatorCurent;
    }

    const string& getStare() const
    {
        return stare;
    }

    void setTabla(const string& tablaNoua)
    {
        tabla = tablaNoua;
    }

    void setJucatorCurent(char jucatorNou)
    {
        jucatorCurent = jucatorNou;
    }

    void setStare(const string& stareNoua)
    {
        stare = stareNoua;
    }

    void setDim(int newDim)
    {
        dim = newDim;
    }
};


#endif //TICTACTOE_JOC_H