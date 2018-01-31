#include "src/h/report/VisitGroupTag.h"

int VisitGroupTag::getFirstIndex()
{
    return firstIndex;
}

void VisitGroupTag::setFirstIndex(int index)
{
    firstIndex = index;
}


int VisitGroupTag::getLastIndex()
{
    return lastIndex;
}

void VisitGroupTag::setLastIndex(int index)
{
    lastIndex = index;
}

QDate VisitGroupTag::getFirstDate()
{
    return firstDate;
}

void VisitGroupTag::setFirstDate(QDate date)
{
    firstDate = date;
}


QDate VisitGroupTag::getLastDate()
{
    return lastDate;
}

void VisitGroupTag::setLastDate(QDate date)
{
    lastDate = date;
}

bool VisitGroupTag::lessThen(VisitGroupTag tag1, VisitGroupTag tag2)
{
    return (tag1.lastDate < tag2.lastDate) ||
            ((tag1.lastDate == tag2.lastDate) && (tag1.firstDate < tag2.firstDate));
}
