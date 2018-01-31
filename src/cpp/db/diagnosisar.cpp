#include "src/h/db/diagnosisar.h"

DiagnosisAR::DiagnosisAR()
{
}

int DiagnosisAR::getId()
{
    return id;
}

void DiagnosisAR::setId(int value)
{
    id = value;
}

QString DiagnosisAR::getName()
{
    return name;
}

void DiagnosisAR::setName(QString value)
{
    name = value;
}

