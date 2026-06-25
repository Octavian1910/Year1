//
// Created by octav on 25.06.2026.
//

#include "Tests.h"
#include "../domain/Joc.h"

#include <assert.h>
#include <iostream>
#include <fstream>

#include "../domain/validator/Validator.h"
#include "../domain//validator/ValidatorException.h"
#include "../repository/Repository.h"
#include "../repository/RepositoryException.h"
#include "../service/Service.h"
#include "../service/ServiceException.h"

void testDomain()
{
    Joc joc{1,3,"X-0X0-X00",'X',"Terminat"};
    assert(joc.getId() == 1);
    assert(joc.getDim() == 3);
    assert(joc.getTabla() == "X-0X0-X00");
    assert(joc.getJucatorCurent() == 'X');
    assert(joc.getStare() == "Terminat");

    //setere
    joc.setJucatorCurent('0');
    joc.setStare("Finalizat");
    joc.setTabla("X-0X0-X00XXXX");
    joc.setDim(4);
    assert(joc.getDim() == 4);
    assert(joc.getTabla() == "X-0X0-X00XXXX");
    assert(joc.getJucatorCurent() == '0');
    assert(joc.getStare() == "Finalizat");
}


void testValidator()
{
    Validator val;
    {
        Joc joc{1,3,"X-0X0-X00",'X',"Terminat"};
        val.validate(joc);
    }

    {
        //toate invalide
        Joc invalid{-5,1,"asd",'a',"asd"};
        try
        {
            val.validate(invalid);
            assert(false);
        }catch (ValidatorException& e)
        {
            assert(e.getMessage() == "Dimensiunea trebuie sa fie 3 ,4 sau 5!\nTabla trebuie sa aiba exact dim*dim caractere!\nTabla poate contine doar 0 X sau - !\nJucatorul curent poate sa fie  X sau 0!\nStarea trebuie sa fie Neinceput , In derulare sau Terminat!\n");
        }

    }
}

void testRepo()
{
    std::ofstream fout("test_repo.txt");

    fout << "1 3 X-0X0-X00 X Terminat\n";
    fout << "2 4 X0--X---0--XX0-- 0 Neinceput\n";
    fout << "3 5 ------------------------- X In derulare\n";

    fout.close();

    Repository repo("test_repo.txt");

    const auto& all = repo.getAll();

    assert(all.size() == 3);
    assert(all[0].getId() == 1);
    assert(all[1].getId() == 2);
    assert(all[2].getId() == 3);

    Joc joc = repo.findById(2);
    assert(joc.getId() == 2);
    assert(joc.getDim() == 4);
    assert(joc.getTabla() == "X0--X---0--XX0--");
    assert(joc.getJucatorCurent() == '0');
    assert(joc.getStare() == "Neinceput");

    Joc nou{4, 3, "---------", 'X', "Neinceput"};
    repo.add(nou);

    assert(repo.getAll().size() == 4);

    Joc gasit = repo.findById(4);
    assert(gasit.getId() == 4);
    assert(gasit.getDim() == 3);
    assert(gasit.getTabla() == "---------");
    assert(gasit.getJucatorCurent() == 'X');
    assert(gasit.getStare() == "Neinceput");

    try
    {
        Joc duplicat{4, 3, "---------", '0', "Neinceput"};
        repo.add(duplicat);
        assert(false);
    }
    catch (RepositoryException&)
    {
        assert(true);
    }

    Joc modificat{2, 4, "XXXX0000--------", 'X', "In derulare"};
    repo.update(modificat);

    Joc jocModificat = repo.findById(2);
    assert(jocModificat.getId() == 2);
    assert(jocModificat.getDim() == 4);
    assert(jocModificat.getTabla() == "XXXX0000--------");
    assert(jocModificat.getJucatorCurent() == 'X');
    assert(jocModificat.getStare() == "In derulare");


    try
    {
        Joc inexistent{100, 3, "---------", 'X', "Neinceput"};
        repo.update(inexistent);
        assert(false);
    }
    catch (RepositoryException&)
    {
        assert(true);
    }
}



void testService()
{
    std::ofstream fout("test_service.txt");

    fout << "1 3 --------- X Neinceput\n";
    fout << "2 3 X0X0X0X0X 0 Terminat\n";
    fout << "3 3 X-------- X In derulare\n";

    fout.close();
    Repository repo{"test_service.txt"};
    Validator val;
    Service serv{repo, val};

    {
        Joc joc = serv.findById(1);

        assert(joc.getId() == 1);
        assert(joc.getDim() == 3);
        assert(joc.getTabla() == "---------");
        assert(joc.getJucatorCurent() == 'X');
        assert(joc.getStare() == "Neinceput");
    }


    {
        auto jocuri = serv.getAllSorted();

        assert(jocuri.size() == 3);
        assert(jocuri[0].getStare() == "Neinceput");
        assert(jocuri[1].getStare() == "In derulare");
        assert(jocuri[2].getStare() == "Terminat");
    }


    {
        serv.addJoc(4, 3, "---------", '0', "Neinceput");

        Joc joc = serv.findById(4);
        assert(joc.getId() == 4);
        assert(joc.getDim() == 3);
        assert(joc.getTabla() == "---------");
        assert(joc.getJucatorCurent() == '0');
        assert(joc.getStare() == "Neinceput");
    }


    {
        serv.updateJoc(4, 3, "X--------", '0', "In derulare");

        Joc joc = serv.findById(4);
        assert(joc.getTabla() == "X--------");
        assert(joc.getJucatorCurent() == '0');
        assert(joc.getStare() == "In derulare");
    }


    {
        serv.makeMove(1, 0);

        Joc joc = serv.findById(1);
        assert(joc.getTabla() == "X--------");
        assert(joc.getJucatorCurent() == '0');
        assert(joc.getStare() == "In derulare");
    }

    {
        try
        {
            serv.makeMove(1, 0);
            assert(false);
        }
        catch (ServiceException&)
        {
            assert(true);
        }
    }

    {
        try
        {
            serv.makeMove(1, 100);
            assert(false);
        }
        catch (ServiceException&)
        {
            assert(true);
        }
    }
}

void Tests::runAll()
{
    testDomain();
    testValidator();
    testRepo();
    testService();
    std::cout << "Al tests have passed!\n";
}
