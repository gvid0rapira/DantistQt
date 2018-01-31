#-------------------------------------------------
#
# Project created by QtCreator 2012-05-20T18:11:32
#
#-------------------------------------------------
#CONFIG   += qaxcontainer
QT       += core gui\
            sql axcontainer

TARGET = DantistQt
TEMPLATE = app


SOURCES += src/cpp/main.cpp\
    src/cpp/mainwindow.cpp \
    src/cpp/db/Connection.cpp \
    src/cpp/employeedialog.cpp \
    src/cpp/employeevisitdialog.cpp \
    src/cpp/diagnosisdialog.cpp \
    src/cpp/diagnostablemodel.cpp \
    src/cpp/db/employeear.cpp \
    src/cpp/db/diagnosisar.cpp \
    src/cpp/db/visitar.cpp \
    src/cpp/report/visitgrouptag.cpp

HEADERS  += src/h/mainwindow.h \
    src/h/db/Connection.h \
    src/h/db/dbstructure.h\
    src/h/employeedialog.h \
    src/h/employeevisitdialog.h \
    src/h/diagnosisdialog.h \
    src/h/diagnostablemodel.h \
    src/h/db/employeear.h \
    src/h/db/diagnosisar.h \
    src/h/db/visitar.h \
    src/h/report/visitgrouptag.h \
    src/h/Currency.h


FORMS    += src/forms/mainwindow.ui \
    src/forms/employeevisitdialog.ui \
    src/forms/diagnosisdialog.ui

OTHER_FILES += \
    src/sql/test_data.sqlce \
    src/sql/schema_sqlite.sql \
    src/sql/schema.sqlce

RESOURCES += \
    resources/resources.qrc
