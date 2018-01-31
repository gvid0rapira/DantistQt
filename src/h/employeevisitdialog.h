#ifndef EMPLOYEEVISITDIALOG_H
#define EMPLOYEEVISITDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QDataWidgetMapper>

#include "src/h/diagnostablemodel.h"
#include "src/h/db/visitar.h"

namespace Ui {
class EmployeeVisitDialog;
}

class EmployeeVisitDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EmployeeVisitDialog(QWidget *parent = 0);
    ~EmployeeVisitDialog();
    /**
     * Указание визита для редактирования.
     */
    void setVisitToEdit(VisitAR *visit);
    /*!
     * Очистка полей формы
     */
    void clear();
signals:
    void visitInserted();
public slots:
    // void initBeforDisplay();
private slots:
    void on_emplLineEdit_textChanged(const QString &arg1);

    void on_emplListView_pressed(const QModelIndex &index);

    void on_acceptBtn_clicked();

    void on_rejectBtn_clicked();

    void selectPatientDiagn();
    void unselectPatientDiagn();

    void on_emplLineEdit_returnPressed();

private:
    Ui::EmployeeVisitDialog *ui;

    QSqlQueryModel    *emplLstModel;
    QSqlQueryModel    *allDiagnLstModel;
    DiagnosTableModel *emplDiagnLstModel;

    VisitAR *visitToEdit;

    void addVisit();
    void updateVisit();
};

#endif // EMPLOYEEVISITDIALOG_H

