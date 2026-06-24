#include "tests/tests.h"
#include "repository/MelodieRepository.h"
#include "domain/validator/ValidatorMelodie.h"
#include "service/MelodieService.h"
#include "ui/GUI.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    tests::testAll();

    MelodieRepository repo{"melodii.txt"};
    ValidatorMelodie validator;
    MelodieService serv{repo, validator};

    GUI gui{serv};
    gui.show();

    return app.exec();
}