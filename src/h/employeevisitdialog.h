#ifndef EMPLOYEEVISITDIALOG_H
#define EMPLOYEEVISITDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class EmployeeVisitDialog;
}

class EmployeeVisitDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmployeeVisitDialog(int row, QSqlRelationalTableModel *m, QWidget *parent = 0);
    ~EmployeeVisitDialog();
    
private slots:
    void on_emplCombo_editTextChanged(const QString &arg1);

private:
    Ui::EmployeeVisitDialog *ui;

    QSqlRelationalTableModel *model;
    QSqlTableModel *relationModel;
};

#endif // EMPLOYEEVISITDIALOG_H