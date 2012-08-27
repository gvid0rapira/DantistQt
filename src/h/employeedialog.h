#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QSqlTableModel>
#include <QDataWidgetMapper>


enum{
    Employee_Id = 0,
    Employee_Lname = 1,
    Employee_Fname = 2,
    Employee_Mname = 3,
    Employee_TabNumber = 4,
    Employee_Dep = 5
};

class EmployeeDialog : public QDialog
{
    Q_OBJECT


public:
    explicit EmployeeDialog(int row, QSqlTableModel *m, QWidget *parent = 0);
    ~EmployeeDialog();

    /**
     * Указание строки модели данных для редактирования.
     */
    void rowToEdit(int row);
private slots:
    void acceptInput();

private:
    //--*- Список элементов интерфейса
    QLabel *lnameLabel;
    QLabel *fnameLabel;
    QLabel *mnameLabel;
    QLabel *tabNumLabel;
    QLabel *depLabel;

    QLineEdit *lnameEdit;
    QLineEdit *fnameEdit;
    QLineEdit *mnameEdit;
    QLineEdit *tabNumEdit;
    QLineEdit *depEdit;

    QPushButton *acceptButton;
    QPushButton *rejectButton;

    //--*- Объекты для работы с данными
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EMPLOYEEDIALOG_H
