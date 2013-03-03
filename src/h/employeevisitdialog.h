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
    explicit EmployeeVisitDialog(int row, QSqlQueryModel *m, QWidget *parent = 0);
    ~EmployeeVisitDialog();
    /**
     * ”казание строки модели данных дл€ редактировани€.
     */
    void rowToEdit(int row);

signals:
    void visitInserted();
public slots:
    void initBeforDisplay();
private slots:
    void on_emplLineEdit_textChanged(const QString &arg1);

    void on_emplListView_pressed(const QModelIndex &index);

    void on_acceptBtn_clicked();

    void on_rejectBtn_clicked();

private:
    Ui::EmployeeVisitDialog *ui;

    QSqlQueryModel *model;
    QSqlQueryModel *emplLstModel;
    QSqlQueryModel *diagnLstModel;
    QDataWidgetMapper *mapper;
};

#endif // EMPLOYEEVISITDIALOG_H
