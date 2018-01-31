#ifndef DIAGNOSTABLEMODEL_H
#define DIAGNOSTABLEMODEL_H

#include <QtSql>

/*!
 * Модель для списка диагнозов пациента. Список диагнозов имеет 2 столбца:
 * 1. id
 * 2. name
 */
class DiagnosTableModel: public QAbstractTableModel
{
public:
    DiagnosTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool appendRow(int id, QString name);
    bool deleteRow(int row);
    QMap<int, QString> *getDiagnosises();
    /*!
     * Удаление всех элементов из модели
     */
    void clear();
private:
    QMap<int, QString> diagnosises;

};

#endif // DIAGNOSTABLEMODEL_H

