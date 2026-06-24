//
// Created by octav on 24.06.2026.
//

#include "ValidatorMelodie.h"

#include "ValidatorException.h"
#include "../domain/Melodie.h"

void ValidatorMelodie::validate(const Melodie &melodie)
{
    string errors;
    if(melodie.get_ID() <= 0)
        errors += "ID must be greater than 0!\n";

    if (melodie.get_title().empty() == true)
        errors += "Title cant be null!\n";

    if (melodie.get_artist().empty() == true)
        errors += "Artist cant be null!\n";

    if (melodie.get_rank() < 0 || melodie.get_rank() > 10)
        errors += "Rank must be between 0 and 10!\n";

    if (errors.empty() == false)
        throw ValidatorException(errors);
}
