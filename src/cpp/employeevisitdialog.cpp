#include "src/h/employeevisitdialog.h"
#include "ui_employeevisitdialog.h"
#include "src/h/db/dbstructure.h"

#include "src/h/mainwindow.h"

#include <QMessageBox>

EmployeeVisitDialog::EmployeeVisitDialog(int row, QSqlQueryModel *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeVisitDialog)
{
    model = m;
    ui->setupUi(this);

    QRegExpValidator *nameValidator
            = new QRegExpValidator(QRegExp(tr("[А-Я][а-я]{1,50}")) ,this);

    ui->emplLineEdit->setValidator(nameValidator);
    emplLstModel = new QSqlQueryModel();
    emplLstModel->setQuery(QString("SELECT id,")
                         + QString("lname || ' ' || fname || ' ' || mname || ', ' || tab_num ")
                         + QString("FROM employee ")
                         + QString("WHERE id = -1")
                         );


    ui->emplListView->setModel(emplLstModel);
    ui->emplListView->setModelColumn(1);

    diagnLstModel = new QSqlQueryModel();
    diagnLstModel->setQuery(QString("SELECT * FROM diagnosis"));
    ui->diagnCBox->setModel(diagnLstModel);
    ui->diagnCBox->setModelColumn(1);

    ui->visitDateDEdit->setDate(QDate::currentDate());
    initBeforDisplay();
}

EmployeeVisitDialog::~EmployeeVisitDialog()
{
    delete ui;
    delete emplLstModel;
    delete diagnLstModel;
}

void EmployeeVisitDialog::on_emplLineEdit_textChanged(const QString &arg1)
{
    if(arg1 != "") {
        QString tmplStr = arg1.left(1).toUpper() + arg1.right(arg1.length() - 1).toLower();
        emplLstModel->setQuery(QString("SELECT id,")
                             + QString("tab_num || ', ' || lname || ' ' || fname || ' ' || mname ")
                             + QString("FROM employee ")
                               + tr("WHERE lname LIKE '%1\%'").arg(tmplStr));
    } else {
        emplLstModel->setQuery(QString("SELECT id,")
                             + QString("lname || ' ' || fname || ' ' || mname || ', ' || tab_num ")
                             + QString("FROM employee ")
                               + QString("WHERE id = -1"));
    }
}

void EmployeeVisitDialog::on_emplListView_pressed(const QModelIndex &index)
{
    // ui->emplLineEdit->setText(index.data().toString());
}


void EmployeeVisitDialog::rowToEdit(int row)
{
    if(row == -1){
        //TODO: Обработать случай добавления визита.
    } else {
         mapper->setCurrentIndex(row);
    }
}

void EmployeeVisitDialog::on_acceptBtn_clicked()
{
    QString errStr;

    int row = ui->emplListView->currentIndex().row();
    int employeeId = -1;
    if(row > -1) {
        employeeId = emplLstModel->index(row, 0).data().toInt();
    } else {
        errStr.append(tr("   * выбрать сотрудника.\n"));
    }

    if(errStr.length() != 0) {
        errStr.insert(0, tr("Необходимо: \n"));
        QMessageBox::warning(this, tr("Ошибка ввода"),errStr);
        return;
    }
    //--*- Диагноз
    int diagnId = diagnLstModel->index(ui->diagnCBox->currentIndex(), 0)
            .data().toInt();
    float uet = ui->uetEdit->text().toFloat();

    //--*- Дата посещения
    QDate date = ui->visitDateDEdit->date();
    QSqlQuery query;
    query.prepare(QString("INSERT INTO visit (employee_id, visit_date, uet, diagnosis_id)")
                  + QString("VALUES (:employee_id, :visit_date, :uet, :diagnosis_id)"));
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":visit_date", date);
    query.bindValue(":uet", uet);
    query.bindValue(":diagnosis_id", diagnId);

    if(!query.exec()){
        QMessageBox::critical(this, tr("Ошибка сохранения в БД"),
                              query.lastError().text());
        qDebug() << "== Error ======================";
        qDebug() << "Number:        " << query.lastError().number();
        qDebug() << "Type:          " << query.lastError().type();
        qDebug() << "Text:          " << query.lastError().text();
        qDebug() << "Datebase Text: " << query.lastError().databaseText();


    }

    visitInserted();
    this->hide();
    return;
}

void EmployeeVisitDialog::on_rejectBtn_clicked()
{
    this->hide();
}

void EmployeeVisitDialog::initBeforDisplay()
{
    ui->emplLineEdit->setFocus();
}
