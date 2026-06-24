#pragma once

#include <QAbstractTableModel>
#include "../service/MelodieService.h"

class MelodieTableModel : public QAbstractTableModel
{
private:
    /*
     * serv: service reference
     */
    MelodieService& serv;

public:
    /*
     * Creates table model
     * :param serv: service reference
     */
    MelodieTableModel(MelodieService& serv)
        : serv{serv} {}

    /*
     * Gets number of rows
     * :return: number of songs
     */
    int rowCount(const QModelIndex& parent) const override
    {
        return static_cast<int>(serv.getAllSorted().size());
    }

    /*
     * Gets number of columns
     * 0 - id
     * 1 - title
     * 2 - artist
     * 3 - rank
     * 4 - number of songs with same rank
     */
    int columnCount(const QModelIndex& parent) const override
    {
        return 5;
    }

    /*
     * Gets data for a table cell
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            auto melodii = serv.getAllSorted();
            const auto& mel = melodii[index.row()];

            if (index.column() == 0)
                return mel.get_ID();

            if (index.column() == 1)
                return QString::fromStdString(mel.get_title());

            if (index.column() == 2)
                return QString::fromStdString(mel.get_artist());

            if (index.column() == 3)
                return mel.get_rank();

            if (index.column() == 4)
                return serv.countRank(mel.get_rank());
        }

        return QVariant();
    }

    /*
     * Gets header names
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            if (section == 0) return "ID";
            if (section == 1) return "Title";
            if (section == 2) return "Artist";
            if (section == 3) return "Rank";
            if (section == 4) return "Same rank";
        }

        return QVariant();
    }

    /*
     * Refreshes model after update/delete
     */
    void refresh()
    {
        beginResetModel();
        endResetModel();
    }
};