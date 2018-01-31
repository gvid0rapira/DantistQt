#ifndef DIAGNOSISAR_H
#define DIAGNOSISAR_H

#include <QString>

class DiagnosisAR
{
public:
    DiagnosisAR();
    int getId();
    void setId(int value);
    QString getName();
    void setName(QString value);
private:
    int id;
    QString name;
};

#endif // DIAGNOSISAR_H

