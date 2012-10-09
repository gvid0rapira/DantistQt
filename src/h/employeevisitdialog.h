#ifndef EMPLOYEEVISITDIALOG_H
#define EMPLOYEEVISITDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QDataWidgetMapper>

namespace Ui {
class EmployeeVisitDialog;
}

class EmployeeVisitDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmployeeVisitDialog(int row, QSqlRelationalTableModel *m, QWidget *parent = 0);
    ~EmployeeVisitDialog();
    /**
     * ”казание строки модели данных дл€ редактировани€.
     */
    void rowToEdit(int row);

private slots:
    void on_emplLineEdit_textChanged(const QString &arg1);

    void on_emplListView_pressed(const QModelIndex &index);

    void on_buttonBox_accepted();

private:
    Ui::EmployeeVisitDialog *ui;

    QSqlRelationalTableModel *model;
    QSqlTableModel *relationModel;
    QDataWidgetMapper *mapper;
};

#endif // EMPLOYEEVISITDIALOG_H
