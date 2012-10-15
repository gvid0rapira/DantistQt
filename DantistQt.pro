#-------------------------------------------------
#
# Project created by QtCreator 2012-05-20T18:11:32
#
#-------------------------------------------------

CONFIG   += qaxcontainer
QT       += core gui\
            sql

TARGET = DantistQt
TEMPLATE = app


SOURCES += src/cpp/main.cpp\
    src/cpp/mainwindow.cpp \
    src/cpp/db/Connection.cpp \
    src/cpp/employeedialog.cpp \
    src/cpp/employeevisitdialog.cpp

HEADERS  += src/h/mainwindow.h \
    src/h/db/Connection.h \
    src/h/db/dbstructure.h\
    src/h/employeedialog.h \
    src/h/employeevisitdialog.h


FORMS    += src/forms/mainwindow.ui \
    src/forms/employeevisitdialog.ui

OTHER_FILES += \
    src/sql/test_data.sqlce \
    src/sql/schema_sqlite.sql \
    src/sql/schema.sqlce

RESOURCES += \
    resources/resources.qrc
