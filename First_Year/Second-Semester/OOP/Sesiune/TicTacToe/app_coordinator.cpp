#include <QApplication>

#include "tests/Tests.h"
#include "repository/Repository.h"
#include "domain/validator/Validator.h"
#include "service/Service.h"
#include "ui/MainGUI.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Tests::runAll();

    Repository repo{"jocuri.txt"};
    Validator val;
    Service serv{repo, val};

    MainGUI gui{serv};
    gui.show();

    return app.exec();
}