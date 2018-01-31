#ifndef EMPLOYEEAR_H
#define EMPLOYEEAR_H

#include <QString>
#include <QDate>

class EmployeeAR
{
public:
    EmployeeAR();
    int getId();
    void setId(int value);
    QString getFname();
    void setFname(QString value);
    QString getLname();
    void setLname(QString value);
    QString getMname();
    void setMname(QString value);
    QDate getBirthDate();
    void setBirthDate(QDate value);
    QString getWorkPlace();
    void setWorkPlace(QString value);
    int getTabNum();
    void setTabNum(int value);

    static EmployeeAR *getEmployeeById(int id);
private:
    int id;
    QString fname;
    QString lname;
    QString mname;
    QDate birthDate;
    QString workPlace;
    int tabNum;
};

#endif // EMPLOYEEAR_H

