#include "src/h/employeevisitdialog.h"
#include "ui_employeevisitdialog.h"
#include "src/h/db/dbstructure.h"

#include "src/h/mainwindow.h"

#include <QMessageBox>

EmployeeVisitDialog::EmployeeVisitDialog(int row, QSqlQueryModel *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeVisitDialog)
{
    ui->setupUi(this);
    model = m;

    emplLstModel = new QSqlQueryModel();
    emplLstModel->setQuery(QString("SELECT id,")
                         + QString("lname || ' ' || fname || ' ' || mname || ', ' || tab_num ")
                         + QString("FROM employee ")
                         + QString("WHERE id = -1")
                         );
    // emplLstModel->setFilter("id = -1");
    // relationModel->setFilter("id = -1");
    // TODO: обрабатывать row при формировании модели списка сотрудников

    ui->emplListView->setModel(emplLstModel);
    ui->emplListView->setModelColumn(1);

    ui->visitTimeDTEdit->setDateTime(QDateTime::currentDateTime());
    //--*- Отображение модели данных на виджеты
    /*mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->emplListView, Visit_Employee_Id);
    mapper->addMapping(ui->visitTimeDTEdit, Visit_Time);

    if(row != -1) // Редактирование
    {
        mapper->setCurrentIndex(row);
    } else {      // Добавление

        mapper->toFirst();
        row = mapper->currentIndex();
        model->insertRow(row);
        mapper->setCurrentIndex(row);
    }*/
}

EmployeeVisitDialog::~EmployeeVisitDialog()
{
    delete ui;
}

void EmployeeVisitDialog::on_emplLineEdit_textChanged(const QString &arg1)
{
    if(arg1 != "") {
        // emplLstModel->setFilter(tr("lname Like '%1\%'").arg(arg1));
        emplLstModel->setQuery(QString("SELECT id,")
                             + QString("tab_num || ', ' || lname || ' ' || fname || ' ' || mname ")
                             + QString("FROM employee ")
                               + tr("WHERE lname LIKE '%1\%'").arg(arg1));
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

void EmployeeVisitDialog::on_buttonBox_accepted()
{
    int row = ui->emplListView->currentIndex().row();
    int employeeId = emplLstModel->index(row, 0).data().toInt();

    QDateTime time = ui->visitTimeDTEdit->dateTime();
    QSqlQuery query;

    query.prepare(QString("INSERT INTO visit (employee_id, visit_time)")
                  + QString("VALUES (:employee_id, :visit_time)"));
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":visit_time", time);

    if(!query.exec()){
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Ошибка. Не удалось сохранить данные."));
        // TODO: Сделать вывод строки через tr()
        msgBox.setDetailedText(query.lastError().text());
        msgBox.exec();
        qDebug() << "== Error ======================";
        qDebug() << "Number:        " << query.lastError().number();
        qDebug() << "Type:          " << query.lastError().type();
        qDebug() << "Text:          " << query.lastError().text();
        qDebug() << "Datebase Text: " << query.lastError().databaseText();


    }
    // Обновим модель
    model->setQuery(MainWindow::getEmplVisitSql());
    this->hide();
}

void EmployeeVisitDialog::rowToEdit(int row)
{
    if(row == -1){
        //TODO: Обработать случай добавления визита.
    } else {
         mapper->setCurrentIndex(row);
    }
}
