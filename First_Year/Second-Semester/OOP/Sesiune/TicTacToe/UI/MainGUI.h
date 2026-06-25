#pragma once

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QGridLayout>

#include "../service/Service.h"
#include "../repository/RepositoryException.h"
#include "../domain/validator/ValidatorException.h"
#include "../service/ServiceException.h"

#include "JocTableModel.h"

class MainGUI : public QWidget
{
private:
    Service& serv;

    // tabel + model
    QTableView* table;
    JocTableModel* model;

    // campuri
    QLineEdit* txtDim;
    QLineEdit* txtTabla;
    QLineEdit* txtJucator;
    QLineEdit* txtStare;

    // butoane
    QPushButton* btnAdd;
    QPushButton* btnUpdate;

    // zona pentru tabla de joc
    QWidget* boardWidget;
    QGridLayout* boardLay;

    // jocul selectat
    int selectedId = -1;

private:
    void initGUI()
    {
        auto* mainLay = new QVBoxLayout;
        setLayout(mainLay);

        // =====================================================
        // 1. tabelul
        // =====================================================
        table = new QTableView;
        model = new JocTableModel(serv);

        table->setModel(model);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        mainLay->addWidget(table);

        // =====================================================
        // 2. formularul
        // =====================================================
        auto* formLay = new QFormLayout;

        txtDim = new QLineEdit;
        txtTabla = new QLineEdit;
        txtJucator = new QLineEdit;
        txtStare = new QLineEdit;

        formLay->addRow("Dim", txtDim);
        formLay->addRow("Tabla", txtTabla);
        formLay->addRow("Jucator", txtJucator);
        formLay->addRow("Stare", txtStare);

        mainLay->addLayout(formLay);

        // =====================================================
        // 3. butoane add/update
        // =====================================================
        auto* btnLay = new QHBoxLayout;

        btnAdd = new QPushButton("Add");
        btnUpdate = new QPushButton("Update");

        btnLay->addWidget(btnAdd);
        btnLay->addWidget(btnUpdate);

        mainLay->addLayout(btnLay);

        // =====================================================
        // 4. tabla de joc
        // =====================================================
        boardWidget = new QWidget;
        boardLay = new QGridLayout;
        boardWidget->setLayout(boardLay);

        mainLay->addWidget(boardWidget);

        resize(1000, 700);
        setWindowTitle("TicTacToe");
    }

    void clearBoard()
    {
        while (boardLay->count() > 0)
        {
            QLayoutItem* item = boardLay->takeAt(0);

            if (item->widget() != nullptr)
            {
                delete item->widget();
            }

            delete item;
        }
    }

    void loadSelected()
    {
        auto selected = table->selectionModel()->selectedRows();

        if (selected.empty())
        {
            selectedId = -1;

            txtDim->clear();
            txtTabla->clear();
            txtJucator->clear();
            txtStare->clear();

            clearBoard();
            return;
        }

        int row = selected[0].row();

        auto jocuri = serv.getAllSorted();
        const auto& joc = jocuri[row];

        selectedId = joc.getId();

        txtDim->setText(QString::number(joc.getDim()));
        txtTabla->setText(QString::fromStdString(joc.getTabla()));
        txtJucator->setText(QString(QChar(joc.getJucatorCurent())));
        txtStare->setText(QString::fromStdString(joc.getStare()));

        loadBoard(joc);
    }

    void loadBoard(const Joc& joc)
    {
        clearBoard();

        int dim = joc.getDim();
        std::string tabla = joc.getTabla();

        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                int poz = i * dim + j;

                QString text = " ";
                if (tabla[poz] != '-')
                {
                    text = QString(QChar(tabla[poz]));
                }

                auto* btn = new QPushButton(text);
                btn->setMinimumSize(60, 60);

                boardLay->addWidget(btn, i, j);

                QObject::connect(btn, &QPushButton::clicked, [=]()
                {
                    if (selectedId == -1)
                        return;

                    try
                    {
                        serv.makeMove(selectedId, poz);
                        reloadAllAfterMove();
                    }
                    catch (ServiceException& e)
                    {
                        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getMessage()));
                    }
                    catch (RepositoryException& e)
                    {
                        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getMessage()));
                    }
                    catch (ValidatorException& e)
                    {
                        QMessageBox::warning(this, "Eroare", QString::fromStdString(e.getMessage()));
                    }
                });
            }
        }
    }

    void reloadAll()
    {
        model->refresh();
        loadSelected();
    }

    void reloadAllAfterMove()
    {
        int oldId = selectedId;
        model->refresh();

        auto jocuri = serv.getAllSorted();

        for (int i = 0; i < (int)jocuri.size(); i++)
        {
            if (jocuri[i].getId() == oldId)
            {
                table->selectRow(i);
                break;
            }
        }

        loadSelected();
    }

    void connectSignals()
    {
        // selectie in tabel
        QObject::connect(table->selectionModel(), &QItemSelectionModel::selectionChanged,
            [&](const QItemSelection&, const QItemSelection&)
            {
                loadSelected();
            });

        // add
        QObject::connect(btnAdd, &QPushButton::clicked, [&]()
        {
            try
            {
                int dim = txtDim->text().toInt();
                std::string tabla = txtTabla->text().toStdString();

                QString qJucator = txtJucator->text();
                if (qJucator.isEmpty())
                {
                    throw ValidatorException("Jucator invalid!");
                }

                char jucator = qJucator[0].toLatin1();

                // id nou = maxId + 1
                int maxId = 0;
                for (const auto& joc : serv.getAll())
                {
                    if (joc.getId() > maxId)
                        maxId = joc.getId();
                }

                int newId = maxId + 1;

                serv.addJoc(newId, dim, tabla, jucator, "Neinceput");
                reloadAll();
            }
            catch (ValidatorException& e)
            {
                QMessageBox::warning(this, "Validation error", QString::fromStdString(e.getMessage()));
            }
            catch (RepositoryException& e)
            {
                QMessageBox::warning(this, "Repository error", QString::fromStdString(e.getMessage()));
            }
        });

        // update
        QObject::connect(btnUpdate, &QPushButton::clicked, [&]()
        {
            if (selectedId == -1)
            {
                QMessageBox::warning(this, "Eroare", "Nu exista joc selectat!");
                return;
            }

            try
            {
                int dim = txtDim->text().toInt();
                std::string tabla = txtTabla->text().toStdString();

                QString qJucator = txtJucator->text();
                if (qJucator.isEmpty())
                {
                    throw ValidatorException("Jucator invalid!");
                }

                char jucator = qJucator[0].toLatin1();
                std::string stare = txtStare->text().toStdString();

                serv.updateJoc(selectedId, dim, tabla, jucator, stare);
                reloadAllAfterMove();
            }
            catch (ValidatorException& e)
            {
                QMessageBox::warning(this, "Validation error", QString::fromStdString(e.getMessage()));
            }
            catch (RepositoryException& e)
            {
                QMessageBox::warning(this, "Repository error", QString::fromStdString(e.getMessage()));
            }
        });
    }

public:
    MainGUI(Service& serv) : serv{serv}
    {
        initGUI();
        connectSignals();
        model->refresh();
    }
};