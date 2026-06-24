#pragma once

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QHeaderView>

#include "../service/MelodieService.h"
#include "../repository/RepositoryException.h"
#include "../domain/validator/ValidatorException.h"
#include "../service/ServiceException.h"

#include "MelodieTableModel.h"
#include "RankBarWidget.h"

class GUI : public QWidget
{
private:
    /*
     * serv: service reference
     */
    MelodieService& serv;

    /*
     * table + model
     */
    QTableView* table;
    MelodieTableModel* model;

    /*
     * edit controls
     */
    QLineEdit* txtTitle;
    QSlider* sliderRank;
    QLabel* lblRankValue;

    /*
     * buttons
     */
    QPushButton* btnUpdate;
    QPushButton* btnDelete;

    /*
     * bottom chart
     */
    RankBarWidget* barWidget;

    /*
     * selected song id
     * -1 means no selection
     */
    int selectedId = -1;

    /*
     * Initializes GUI
     */
    void initGUI()
    {
        auto* mainLay = new QVBoxLayout;
        setLayout(mainLay);

        // =========================
        // TABLE
        // =========================
        table = new QTableView;
        model = new MelodieTableModel(serv);
        table->setModel(model);

        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        mainLay->addWidget(table);

        // =========================
        // FORM
        // =========================
        auto* formLay = new QFormLayout;

        txtTitle = new QLineEdit;
        formLay->addRow("Title", txtTitle);

        sliderRank = new QSlider(Qt::Horizontal);
        sliderRank->setMinimum(0);
        sliderRank->setMaximum(10);
        sliderRank->setValue(0);

        lblRankValue = new QLabel("0");

        auto* rankLay = new QHBoxLayout;
        rankLay->addWidget(sliderRank);
        rankLay->addWidget(lblRankValue);

        formLay->addRow("Rank", rankLay);

        mainLay->addLayout(formLay);

        // =========================
        // BUTTONS
        // =========================
        auto* btnLay = new QHBoxLayout;

        btnUpdate = new QPushButton("Update");
        btnDelete = new QPushButton("Delete");

        btnLay->addWidget(btnUpdate);
        btnLay->addWidget(btnDelete);

        mainLay->addLayout(btnLay);

        // =========================
        // BAR CHART
        // =========================
        barWidget = new RankBarWidget(serv);
        mainLay->addWidget(barWidget);

        resize(1000, 700);
        setWindowTitle("Melodii GUI");
    }

    /*
     * Reloads model and repaints bars
     */
    void reloadAll()
    {
        model->refresh();
        barWidget->update();
    }

    /*
     * Loads selected song in controls
     */
    void loadSelected()
    {
        auto selected = table->selectionModel()->selectedRows();

        if (selected.empty())
        {
            selectedId = -1;
            txtTitle->clear();
            sliderRank->setValue(0);
            lblRankValue->setText("0");
            return;
        }

        int row = selected[0].row();

        auto melodii = serv.getAllSorted();
        const auto& mel = melodii[row];

        selectedId = mel.get_ID();
        txtTitle->setText(QString::fromStdString(mel.get_title()));
        sliderRank->setValue(mel.get_rank());
        lblRankValue->setText(QString::number(mel.get_rank()));
    }

    /*
     * Connects signals
     */
    void connectSignals()
    {
        // update rank label when slider changes
        QObject::connect(sliderRank, &QSlider::valueChanged,
            [&](int value)
            {
                lblRankValue->setText(QString::number(value));
            });

        // when selection changes, load selected song
        QObject::connect(table->selectionModel(), &QItemSelectionModel::selectionChanged,
            [&](const QItemSelection& selected, const QItemSelection& deselected)
            {
                loadSelected();
            });

        // update button
        QObject::connect(btnUpdate, &QPushButton::clicked,
    [&]()
    {
        if (selectedId == -1)
        {
            QMessageBox::warning(this, "Error", "No song selected!");
            return;
        }

        string newTitle = txtTitle->text().toStdString();
        int newRank = sliderRank->value();

        try
        {
            serv.update(selectedId, newTitle, newRank);

            reloadAll();
            loadSelected();
        }
        catch (ValidatorException& e)
        {
            QMessageBox::warning(this, "Validation error",
                QString::fromStdString(e.getMessage()));
        }
        catch (RepositoryException& e)
        {
            QMessageBox::warning(this, "Repository error",
                QString::fromStdString(e.getMessage()));
        }
    });

        // delete button
        QObject::connect(btnDelete, &QPushButton::clicked,
            [&]()
            {
                if (selectedId == -1)
                {
                    QMessageBox::warning(this, "Error", "No song selected!");
                    return;
                }

                try
                {
                    serv.remove(selectedId);

                    selectedId = -1;
                    txtTitle->clear();
                    sliderRank->setValue(0);
                    lblRankValue->setText("0");

                    reloadAll();
                }
                catch (ServiceException& e)
                {
                    QMessageBox::warning(this, "Delete error",
                        QString::fromStdString(e.getMessage()));
                }
                catch (RepositoryException& e)
                {
                    QMessageBox::warning(this, "Repository error",
                        QString::fromStdString(e.getMessage()));
                }
            });
    }

public:
    /*
     * Creates GUI
     * :param serv: service reference
     */
    GUI(MelodieService& serv)
        : serv{serv}
    {
        initGUI();
        connectSignals();
        reloadAll();
    }
};