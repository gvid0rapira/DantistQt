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
    relationModel->setFilter("id = -1");
    // TODO: обрабатывать row при формировании модели списка сотрудников

    ui->emplListView->setModel(relationModel);
    ui->emplListView->setModelColumn(relationModel->fieldIndex("lname"));

}

EmployeeVisitDialog::~EmployeeVisitDialog()
{
    delete ui;
}

void EmployeeVisitDialog::on_emplLineEdit_textChanged(const QString &arg1)
{
    relationModel->setFilter(tr("lname Like '%1\%'").arg(arg1));

}

void EmployeeVisitDialog::on_emplListView_pressed(const QModelIndex &index)
{

}
