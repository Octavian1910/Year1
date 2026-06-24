#pragma once
#include <string>
using std::string;

class Melodie
{
private:
    int id;
    string title;
    string artist;
    int rank;

public:
    Melodie(int id,string title,string artist,int rank) : id(id),title(title),artist(artist),rank(rank){}
    Melodie(const Melodie& ot) : id(ot.id), title(ot.title),artist(ot.artist),rank(ot.rank) {}
    int get_ID() const;
    int get_rank() const;
    const string& get_title() const;
    const string& get_artist() const;
    void set_title(const string& new_title);
    void set_artist(const string& new_artist);
    void set_rank(int new_rank);

};

