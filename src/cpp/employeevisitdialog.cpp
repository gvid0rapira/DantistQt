#include "src/h/employeevisitdialog.h"
#include "ui_employeevisitdialog.h"
#include "src/h/db/dbstructure.h"

EmployeeVisitDialog::EmployeeVisitDialog(int row, QSqlRelationalTableModel *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeVisitDialog)
{
    ui->setupUi(this);
    model = m;

    relationModel = model->relationModel(Visit_Employee_Id);

    // TODO: обрабатывать row при формировании модели списка сотрудников
    relationModel->setFilter(tr("lname='Попов'"));
    ui->emplCombo->setModel(relationModel);
    ui->emplCombo->setModelColumn(relationModel->fieldIndex("lname"));
}

EmployeeVisitDialog::~EmployeeVisitDialog()
{
    delete ui;
}

void EmployeeVisitDialog::on_emplCombo_editTextChanged(const QString &arg1)
{
    const QString filterStr = "lname='ike '" + arg1 + "'";
    relationModel->setFilter(filterStr);
    //relationModel->select();
}
