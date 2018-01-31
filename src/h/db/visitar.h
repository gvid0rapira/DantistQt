#ifndef VISITAR_H
#define VISITAR_H

#include <QDate>
#include <QList>
#include <src/h/db/diagnosisar.h>
#include <src/h/db/employeear.h>

class VisitAR
{
public:
    VisitAR();
    ~VisitAR();
    int getId();
    void setId(int val);
    QDate getDate();
    void setDate(QDate val);
    float getUet();
    void setUet(float val);
    EmployeeAR *getEmployee();
    void setEmployee(EmployeeAR* val);
    QList<DiagnosisAR> getDiagnosises();
    void setDiagnosises(QList<DiagnosisAR> list);

    static VisitAR *getVisitById(int id);
    static QList<VisitAR*> getVisits4Period(QDate from, QDate to);
    static bool lessThanByDate(VisitAR *visit1, VisitAR *visit2);
    bool remove();

    QString getDiagnsAsStr();
    bool isOfTheSameEmpl(const VisitAR &visit);
private:
    int id;
    QDate date;
    float uet;
    EmployeeAR *employee;
    // Хотя бы 1 диагноз есть всегда.
    QList<DiagnosisAR> diagnosises;
};

#endif // VISITAR_H

