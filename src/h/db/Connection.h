#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QObject>

class Connection
{
public:
    QSqlDatabase getConnection();
};

#endif // CONNECTION_H

