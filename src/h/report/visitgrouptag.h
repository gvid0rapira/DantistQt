#ifndef VISITGROUPTAG_H
#define VISITGROUPTAG_H

#include <QDate>

class VisitGroupTag
{
public:
    int getFirstIndex();
    void setFirstIndex(int index);

    int getLastIndex();
    void setLastIndex(int index);

    QDate getFirstDate();
    void setFirstDate(QDate date);

    QDate getLastDate();
    void setLastDate(QDate date);

    static bool lessThen(VisitGroupTag tag1, VisitGroupTag tag2);
private:
    int firstIndex;
    int lastIndex;

    QDate firstDate;
    QDate lastDate;
};

#endif // VISITGROUPTAG_H
