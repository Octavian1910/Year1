#pragma once

#include <QAbstractTableModel>
#include "../service/Service.h"

class JocTableModel : public QAbstractTableModel
{
private:
    Service& serv;

public:
    JocTableModel(Service& serv) : serv{serv} {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        return (int)serv.getAllSorted().size();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override
    {
        return 5;
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole)
        {
            auto jocuri = serv.getAllSorted();
            const auto& joc = jocuri[index.row()];

            switch (index.column())
            {
                case 0:
                    return joc.getId();

                case 1:
                    return joc.getDim();

                case 2:
                    return QString::fromStdString(joc.getTabla());

                case 3:
                    return QString(QChar(joc.getJucatorCurent()));

                case 4:
                    return QString::fromStdString(joc.getStare());
            }
        }

        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0: return "ID";
                case 1: return "Dim";
                case 2: return "Tabla";
                case 3: return "Jucator";
                case 4: return "Stare";
            }
        }

        return QVariant();
    }

    void refresh()
    {
        beginResetModel();
        endResetModel();
    }
};