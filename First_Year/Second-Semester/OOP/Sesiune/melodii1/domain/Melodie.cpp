//
// Created by octav on 24.06.2026.
//

#include "Melodie.h"

const string &Melodie::get_artist() const
{
    return artist;
}

int Melodie::get_ID() const
{
    return id;
}

int Melodie::get_rank() const
{
    return rank;
}

const string &Melodie::get_title() const
{
    return title;
}

void Melodie::set_artist(const string &new_artist)
{
    artist = new_artist;
}

void Melodie::set_rank(int new_rank)
{
    rank = new_rank;
}

void Melodie::set_title(const string &new_title)
{
    title = new_title;
}
