#include "src/h/db/Connection.h"
#include <QMessageBox>

QSqlDatabase Connection::getConnection()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("../DantistQt/DB/dantist.db");
    // sdb.setDatabaseName("DB/dantist.db");
    if(!sdb.open()){
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("������. �� ����������� ���������� � ��."));
        // TODO: ������� ����� ������ ����� tr()
        msgBox.setDetailedText(sdb.lastError().text());
        msgBox.exec();
        qDebug() << sdb.lastError().text();
        // TODO: �������� �� ���-������ �����������
        throw 96;
    }

    return sdb;
}
