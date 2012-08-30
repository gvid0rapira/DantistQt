#include <QtGui/QApplication>
#include "src/h/mainwindow.h"
#include "src/h/db/Connection.h"

int main(int argc, char *argv[])
{
    // Установка кодировки для строк, отображаемых приложением с помощью tr.
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));

    QApplication a(argc, argv);

    Connection cnt;
    try {
        QSqlDatabase sdb = cnt.getConnection();
    } catch(...) {
        return 0;
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}
