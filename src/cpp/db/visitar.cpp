#include "src/h/db/visitar.h"

#include <QtSql>
#include <src/h/db/dbstructure.h>

VisitAR::VisitAR()
{
    id = 0;
    employee = 0;
}

VisitAR::~VisitAR()
{
    delete employee;
}

int VisitAR::getId()
{
    return id;
}

void VisitAR::setId(int val)
{
    id = val;
}

QDate VisitAR::getDate()
{
    return date;
}

void VisitAR::setDate(QDate val)
{
    date = val;
}

float VisitAR::getUet()
{
    return uet;
}

void VisitAR::setUet(float val)
{
    uet = val;
}

EmployeeAR *VisitAR::getEmployee()
{
    return employee;
}

void VisitAR::setEmployee(EmployeeAR *val)
{
    employee = val;
}

QList<DiagnosisAR> VisitAR::getDiagnosises()
{
    return diagnosises;
}

void VisitAR::setDiagnosises(QList<DiagnosisAR> list)
{
    diagnosises = list;
}

VisitAR *VisitAR::getVisitById(int id)
{
    QString visitSql = QString("SELECT * FROM visit WHERE id = %1").arg(id);
    QSqlQuery visitQuery;
    visitQuery.exec(visitSql);

    if(!visitQuery.next()) {return 0;}
    VisitAR *visit = new VisitAR();
    visit->setId( visitQuery.value(Visit_Id).toInt() );
    visit->setDate( visitQuery.value(Visit_Date).toDate() );
    visit->setUet( visitQuery.value(Visit_UET).toFloat() );
    // Выборка сотрудника
    int employee_id = visitQuery.value(Visit_Employee_Id).toInt();
    EmployeeAR *employee = EmployeeAR::getEmployeeById(employee_id);
    visit->setEmployee(employee);
    // Выборка диагнозов
    QString diagnSql = QString("SELECT d.id, d.name FROM visit_diagnosis vd, diagnosis d ")
            + QString("WHERE vd.visit_id = %1 AND d.id = vd.diagnosis_id")
            .arg(visit->getId());
    QSqlQuery diagnQuery;
    diagnQuery.exec(diagnSql);

    QList<DiagnosisAR> diagnosises;
    while( diagnQuery.next() ) {
        DiagnosisAR diagnosis;
        diagnosis.setId( diagnQuery.value(0).toInt() );
        diagnosis.setName( diagnQuery.value(1).toString() );
        diagnosises.append(diagnosis);
    }

    visit->setDiagnosises(diagnosises);

    return visit;
}

QList<VisitAR*> VisitAR::getVisits4Period(QDate from, QDate to)
{
    QString visitSql =  QString("SELECT id FROM visit ")
             + QString("WHERE visit_date BETWEEN \"%1\" AND \"%2\"")
            .arg(from.toString("yyyy-MM-dd"))
            .arg(to.toString("yyyy-MM-dd"));
    QSqlQuery visitQuery;

    if( !visitQuery.exec(visitSql) )
    {
        // TODO: обработать ошибку
    }
    QList<VisitAR*> visits;
    while(visitQuery.next()) {
        VisitAR *visit = getVisitById(visitQuery.value(0).toInt());
        visits.append(visit);
    }
    return visits;
}

bool VisitAR::lessThanByDate(VisitAR *visit1, VisitAR *visit2)
{
    return visit1->getDate() < visit2->getDate();
}

bool VisitAR::remove()
{
    // TODO: сделать транзакцию
    QString delVDiagnSql = QString("DELETE FROM visit_diagnosis WHERE visit_id = %1").arg(id);
    QSqlQuery delVDiagnQuery;
    delVDiagnQuery.exec(delVDiagnSql);

    QString delVisitSql = QString("DELETE FROM visit WHERE id = %1").arg(id);
    QSqlQuery delVisitQuery;
    delVisitQuery.exec(delVisitSql);

    id = 0; // Не существует в БД

    return true;
}

QString VisitAR::getDiagnsAsStr()
{
    QListIterator<DiagnosisAR> diagnIt(diagnosises);
    QString diagns;
    while( diagnIt.hasNext() ){
        DiagnosisAR diagn = diagnIt.next();
        diagns += diagn.getName();
        if( diagnIt.hasNext() ){
            diagns += ", ";
        }
    }

    return diagns;
}

bool VisitAR::isOfTheSameEmpl(const VisitAR &visit)
{
    return employee->getTabNum() == visit.employee->getTabNum();
}

