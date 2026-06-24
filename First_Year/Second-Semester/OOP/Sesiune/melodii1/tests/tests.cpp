//
// Created by octav on 24.06.2026.
//

#include "tests.h"

#include <iostream>
#include <fstream>
#include <qnamespace.h>

#include "assert.h"
#include "../domain/Melodie.h"
#include "../domain/validator/ValidatorException.h"
#include "../domain/validator/ValidatorMelodie.h"
#include "../repository/MelodieRepository.h"
#include "../repository/RepositoryException.h"
#include "../Service/MelodieService.h"
#include "../Service/ServiceException.h"
using std::cout;


void testDomain()
{
    Melodie m1{1,"Believer","Imagine Dragons",8};
    Melodie m2{2,"Numb","Linkin Park",10};

    assert(m1.get_ID() == 1);
    assert(m1.get_title() == "Believer");
    assert(m1.get_artist() == "Imagine Dragons");
    assert(m1.get_rank() == 8);

    m1.set_title("Numb");
    m1.set_artist("Linkin Park");
    m1.set_rank(10);

    assert(m1.get_ID() == 1);
    assert(m1.get_title() == m2.get_title());
    assert(m1.get_artist() == m2.get_artist());
    assert(m1.get_rank() == m2.get_rank());
}


void testValidator()
{
    Melodie valid{1,"Believer","Imagine Dragons",8};
    try
    {
        ValidatorMelodie::validate(valid);
        assert(true);
    }catch (ValidatorException&)
    {
        assert(false);
    }

    Melodie invalida{-5,"","",15};
    try
    {
        ValidatorMelodie::validate(invalida);
        assert(false);
    }catch (ValidatorException& e)
    {
        assert(e.getMessage() == "ID must be greater than 0!\nTitle cant be null!\nArtist cant be null!\nRank must be between 0 and 10!\n");

    }

}


void testRepository()
{
    std::ofstream out("test_melodii.txt");
    out << "1,Believer,Imagine Dragons,8\n";
    out << "2,Numb,Linkin Park,10\n";
    out << "3,Demons,Imagine Dragons,7\n";
    out.close();

    MelodieRepository repo{"test_melodii.txt"};
    assert(repo.getAll().size() == 3);
    const Melodie& m1 = repo.find(1);
    assert(m1.get_ID() == 1);
    assert(m1.get_title() == "Believer");
    assert(m1.get_artist() == "Imagine Dragons");

    try
    {
        repo.find(100);
        assert(false);
    }catch (RepositoryException&)
    {
        assert(true);
    }

    Melodie updated{1,"Updated","Lol",9};
    repo.update(updated);
    const Melodie& m1_updated = repo.find(1);
    assert(m1_updated.get_artist() == "Lol");
    assert(m1_updated.get_rank() == 9);
    assert(m1_updated.get_title() == "Updated");

    try
    {
        Melodie invalid{100,"no","idk",2};
        repo.update(invalid);
        assert(false);
    }catch (RepositoryException&)
    {
        assert(true);
    }
    std::remove("test_service_melodii.txt");
}

void testService()
{
    std::ofstream out("test_service_melodii.txt");
    out << "1,Believer,Imagine Dragons,8\n";
    out << "2,Numb,Linkin Park,10\n";
    out << "3,Demons,Imagine Dragons,7\n";
    out << "4,In The End,Linkin Park,10\n";
    out << "5,Radioactive,Imagine Dragons,8\n";
    out.close();

    MelodieRepository repository{"test_service_melodii.txt"};
    ValidatorMelodie validator_melodie;
    MelodieService{repository,validator_melodie};
    MelodieService serv{repository,validator_melodie};
    assert(serv.getAllSorted().size() == 5);

    //find
    const Melodie& m = serv.find(1);
    assert(m.get_title() == "Believer");


    //sort by rank
    auto all = serv.getAllSorted();
    for (int i = 0 ; i < serv.getAllSorted().size() - 1 ; ++i)
    {
        assert(all[i].get_rank() <= all[i+1].get_rank());
    }

    //update
    try
    {
        serv.update(1,"BadSong",20);
        assert(false);
    }catch (ValidatorException&)
    {
        assert(true);
    }


    //count rank
    assert(serv.countRank(10) == 2);

    auto freq = serv.rankFrequency();
    assert(freq.size() == 11);
    assert(freq[10] == 2);

    //remove
    serv.remove(2);
    assert(serv.getAllSorted().size() == 4);

    try
    {
        serv.remove(4);
        assert(false);
    }catch (ServiceException&)
    {
        assert(true);
    }

    try
    {
        serv.remove(1000);
        assert(false);
    }catch (RepositoryException&)
    {
        assert(true);
    }


    remove("test_service_mmelodii.txt");
}

void tests::testAll()
{
    testDomain();
    testValidator();
    testRepository();
    testService();
    cout << "All test have passed!";
}
