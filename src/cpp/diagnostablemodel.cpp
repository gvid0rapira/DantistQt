#include "src/h/diagnostablemodel.h"

DiagnosTableModel::DiagnosTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

int DiagnosTableModel::rowCount(const QModelIndex &parent) const
{
    return diagnosises.count();
}

int DiagnosTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant DiagnosTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();
    if (role == Qt::TextAlignmentRole) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if (role == Qt::DisplayRole) {

        int id = (diagnosises.begin() + index.row()).key();
        if(index.column() == 0) {
            return id;
        } else {
            return diagnosises.value(id);
        }
    }

    return QVariant();
}

QVariant DiagnosTableModel::headerData(
                int section,
                Qt::Orientation orientation,
                int role) const
{
    if(section == 1)
        return "id";
    else
        return "name";
}

bool DiagnosTableModel::appendRow(int id, QString name)
{
    beginResetModel();
    diagnosises.insert(id, name);
    endResetModel();
    return true;
}

bool DiagnosTableModel::deleteRow(int row)
{
    int id = (diagnosises.begin() + row).key();
    beginResetModel();
    diagnosises.remove(id);
    endResetModel();
    return true;
}

QMap<int, QString> *DiagnosTableModel::getDiagnosises()
{
    return &diagnosises;
}

void DiagnosTableModel::clear()
{
    diagnosises.clear();
}

