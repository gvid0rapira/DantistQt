#include "src/h/db/Connection.h"
#include <QMessageBox>

QSqlDatabase Connection::getConnection()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(QObject::tr(
                    #ifdef QT_DEBUG
                        "%1\\..\\DantistQt\\DB\\dantist.db"
                    #else
                        "%1\\DB\\dantist.db"
                    #endif
                        ).arg(QDir::currentPath()));
    //sdb.setDatabaseName(QObject::tr("%1\\DB\\dantist.db")
      //.arg(QDir::currentPath()));
    if(!sdb.open()){
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Ошибка. Не установлено соединение с БД."));
        // TODO: Сделать вывод строки через tr()
        msgBox.setDetailedText(sdb.lastError().text());
        msgBox.exec();
        qDebug() << sdb.lastError().text();
        // TODO: Заменить на что-нибудь осмысленное
        throw 96;
    }

    return sdb;
}

