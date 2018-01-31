#include "src/h/db/employeear.h"

#include <QtSql>

EmployeeAR::EmployeeAR()
{
    id = 0;
}

int EmployeeAR::getId()
{
    return id;
}

void EmployeeAR::setId(int value)
{
    id = value;
}

QString EmployeeAR::getFname()
{
    return fname;
}

void EmployeeAR::setFname(QString value)
{
    fname = value;
}

QString EmployeeAR::getLname()
{
    return lname;
}

void EmployeeAR::setLname(QString value)
{
    lname = value;
}

QString EmployeeAR::getMname()
{
    return mname;
}

void EmployeeAR::setMname(QString value)
{
    mname = value;
}

QDate EmployeeAR::getBirthDate()
{
    return birthDate;
}

void EmployeeAR::setBirthDate(QDate value)
{
    birthDate = value;
}

QString EmployeeAR::getWorkPlace()
{
    return workPlace;
}

void EmployeeAR::setWorkPlace(QString value)
{
    workPlace = value;
}

int EmployeeAR::getTabNum()
{
    return tabNum;
}

void EmployeeAR::setTabNum(int value)
{
    tabNum = value;
}

EmployeeAR *EmployeeAR::getEmployeeById(int id)
{
    QString sql = QString("SELECT * FROM employee WHERE Id = %1").arg(id);
    QSqlQuery query;
    query.exec(sql);
    // TODO: проверить на ошибки выполнения запроса.
    if(!query.next()) // Не выбрано ни одной строки
    {
        return 0;
    }
    EmployeeAR *empl = new EmployeeAR();
    empl->setId(query.value(0).toInt());
    empl->setLname(query.value(1).toString());
    empl->setFname(query.value(2).toString());
    empl->setMname(query.value(3).toString());
    empl->setBirthDate(query.value(4).toDate());
    empl->setTabNum(query.value(5).toInt());
    empl->setWorkPlace(query.value(6).toString());

    return empl;
}


