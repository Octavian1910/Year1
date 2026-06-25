#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>

#include "../service/Service.h"
#include "../service/Observer.h"
#include "../service/Observable.h"
#include "../service/ServiceException.h"
#include "../repository/RepositoryException.h"
#include "../domain/validator/ValidatorException.h"

class GameWindow : public QWidget, public Observer
{
private:
    Service& serv;
    int idJoc;

    QLabel* lblInfo;
    QTableWidget* table;

private:
    void initGUI()
    {
        auto* mainLay = new QVBoxLayout;
        setLayout(mainLay);

        lblInfo = new QLabel;
        mainLay->addWidget(lblInfo);

        table = new QTableWidget;
        mainLay->addWidget(table);

        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::SingleSelection);

        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        resize(400, 400);
        setWindowTitle("Game Window");
    }

    void loadGame()
    {
        Joc joc = serv.findById(idJoc);

        std::string text =
                "ID: " + std::to_string(joc.getId()) +
                " | Stare: " + joc.getStare() +
                " | Jucator curent: " + std::string(1, joc.getJucatorCurent());

        lblInfo->setText(QString::fromStdString(text));

        int dim = joc.getDim();
        std::string tabla = joc.getTabla();

        table->clear();
        table->setRowCount(dim);
        table->setColumnCount(dim);

        for (int i = 0; i < (int)tabla.size(); i++)
        {
            int linie = i / dim;
            int coloana = i % dim;

            QString textCelula;
            if (tabla[i] == '-')
                textCelula = "";
            else
                textCelula = QString(tabla[i]);

            auto* item = new QTableWidgetItem(textCelula);
            item->setTextAlignment(Qt::AlignCenter);

            table->setItem(linie, coloana, item);
        }
    }

    void connectSignals()
    {
        QObject::connect(table, &QTableWidget::cellClicked,
            [&](int row, int column)
            {
                try
                {
                    Joc joc = serv.findById(idJoc);
                    int dim = joc.getDim();

                    int pozitie = row * dim + column;

                    serv.makeMove(idJoc, pozitie);
                }
                catch (ServiceException& e)
                {
                    QMessageBox::warning(this, "Eroare",
                        QString::fromStdString(e.getMessage()));
                }
                catch (RepositoryException& e)
                {
                    QMessageBox::warning(this, "Eroare",
                        QString::fromStdString(e.getMessage()));
                }
                catch (ValidatorException& e)
                {
                    QMessageBox::warning(this, "Eroare",
                        QString::fromStdString(e.getMessage()));
                }
            });
    }

public:
    GameWindow(Service& serv, int idJoc)
        : serv{serv}, idJoc{idJoc}
    {
        serv.addObserver(this);

        initGUI();
        loadGame();
        connectSignals();
    }

    void update() override
    {
        loadGame();
    }

    ~GameWindow() override
    {
        serv.removeObserver(this);
    }
};