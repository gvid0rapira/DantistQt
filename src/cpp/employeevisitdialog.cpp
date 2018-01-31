#include "src/h/employeevisitdialog.h"
#include "ui_employeevisitdialog.h"
#include "src/h/db/dbstructure.h"
#include "src/h/db/visitar.h"

#include "src/h/mainwindow.h"

#include <QMessageBox>

EmployeeVisitDialog::EmployeeVisitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeeVisitDialog)
{
    ui->setupUi(this);

    visitToEdit = 0;

    QRegExpValidator *nameValidator
            = new QRegExpValidator(QRegExp(tr("[А-Я,а-я]{1,50}")) ,this);


    ui->emplLineEdit->setValidator(nameValidator);
    ui->emplLineEdit->setFocus();

    QDoubleValidator *uetValidator = new QDoubleValidator();
    uetValidator->setLocale(QLocale(QLocale::Russian, QLocale::RussianFederation));
    ui->uetEdit->setValidator(uetValidator);

    emplLstModel = new QSqlQueryModel();
    emplLstModel->clear();
    emplLstModel->setQuery(QString("SELECT id,")
                         + QString("lname || ' ' || fname || ' ' || mname || ', ' || tab_num ")
                         + QString("FROM employee ")
                         + QString("WHERE id = -1")
                         );


    ui->emplListView->setModel(emplLstModel);
    ui->emplListView->setModelColumn(1);

    allDiagnLstModel = new QSqlQueryModel();
    allDiagnLstModel->setQuery(QString("SELECT * FROM diagnosis ORDER BY name"));

    ui->allDiagnListView->setModel(allDiagnLstModel);
    ui->allDiagnListView->setModelColumn(1);

    emplDiagnLstModel = new DiagnosTableModel();

    ui->emplDiagnListView->setModel(emplDiagnLstModel);
    ui->emplDiagnListView->setModelColumn(1);

    connect(ui->selectDiagnBtn, SIGNAL(clicked()), this, SLOT(selectPatientDiagn()));
    connect(ui->allDiagnListView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(selectPatientDiagn()));
    connect(ui->allDiagnListView, SIGNAL(activated(QModelIndex)), this, SLOT(selectPatientDiagn()));
    connect(ui->unselectDiagnBtn, SIGNAL(clicked()), this, SLOT(unselectPatientDiagn()));
    connect(ui->emplDiagnListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(unselectPatientDiagn()));
    connect(ui->emplDiagnListView, SIGNAL(activated(QModelIndex)), this, SLOT(unselectPatientDiagn()));

    ui->visitDateDEdit->setDate(QDate::currentDate());

    // initBeforDisplay();
}

EmployeeVisitDialog::~EmployeeVisitDialog()
{
    delete ui;
    delete emplLstModel;
    delete allDiagnLstModel;
    delete emplDiagnLstModel;

    if( visitToEdit ) { delete visitToEdit; }
}

void EmployeeVisitDialog::setVisitToEdit(VisitAR *visit)
{
    if( visitToEdit == 0 ) { delete visitToEdit; } // Работает ли это?!
    visitToEdit = visit;

    clear();

    if( visit->getId() != 0 ) // Редактирование существующего
    {
        int emplId = visit->getEmployee()->getId();
        emplLstModel->setQuery(QString("SELECT id,")
                               + QString("lname || ' ' || fname || ' ' || mname || ', ' || tab_num ")
                               + QString("FROM employee ")
                               + QString("WHERE id = %1").arg(emplId));
        ui->emplListView->selectionModel()->setCurrentIndex(emplLstModel->index(0, 1),
                                                            QItemSelectionModel::Select);
        QList<DiagnosisAR> diagnosises = visit->getDiagnosises();
        QListIterator<DiagnosisAR> i (diagnosises);
        while( i.hasNext() ) {
            DiagnosisAR diagnosis = i.next();
            emplDiagnLstModel->appendRow(diagnosis.getId(),
                                         diagnosis.getName());
        }
        // TODO: использовать глабальные настройки локали
        QLocale locale(QLocale::Russian, QLocale::RussianFederation);// Для форматирования чисел
        // ui->uetEdit->setText(QString( "%1" ).arg( visit->getUet(), 0, 'g', -1 ));
        ui->uetEdit->setText(locale.toString(visit->getUet()));
        ui->visitDateDEdit->setDate(visit->getDate());
    }
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


void EmployeeVisitDialog::clear()
{
    ui->emplLineEdit->clear();
    emplLstModel->clear();
    emplDiagnLstModel->clear();
    ui->uetEdit->clear();
    ui->visitDateDEdit->setDate(QDate::currentDate());

    ui->emplLineEdit->setFocus();
}

void EmployeeVisitDialog::on_acceptBtn_clicked()
{
    if( visitToEdit->getId() == 0 ) {
        addVisit();
    } else {
        updateVisit();
    }
    visitInserted();
    this->hide();
}

void EmployeeVisitDialog::on_rejectBtn_clicked()
{
    this->hide();
}

void EmployeeVisitDialog::selectPatientDiagn()
{
    int row = ui->allDiagnListView->currentIndex().row();
    if(!(row > -1)) return;
    int diagnId;
    QString diagnName;
    diagnId = allDiagnLstModel->index(row, 0).data().toInt();
    diagnName = allDiagnLstModel->index(row, 1).data().toString();

    emplDiagnLstModel->appendRow(diagnId, diagnName);
}

void EmployeeVisitDialog::unselectPatientDiagn()
{
    int row = ui->emplDiagnListView->currentIndex().row();
    emplDiagnLstModel->deleteRow(row);
}

void EmployeeVisitDialog::addVisit()
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
    QLocale locale(QLocale::Russian, QLocale::RussianFederation);
    float uet = locale.toFloat(ui->uetEdit->text());

    //--*- Дата посещения
    QDate date = ui->visitDateDEdit->date();

    QSqlQuery beginTrans;
    beginTrans.exec("BEGIN TRANSACTION");

    QSqlQuery query;
    query.prepare(QString("INSERT INTO visit (employee_id, visit_date, uet)")
                  + QString("VALUES (:employee_id, :visit_date, :uet)"));
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":visit_date", date);
    query.bindValue(":uet", uet);

    if(!query.exec()){
        QMessageBox::critical(this, tr("Ошибка сохранения в БД"),
                              query.lastError().text());
        qDebug() << "== Error ======================";
        qDebug() << "Number:        " << query.lastError().number();
        qDebug() << "Type:          " << query.lastError().type();
        qDebug() << "Text:          " << query.lastError().text();
        qDebug() << "Datebase Text: " << query.lastError().databaseText();


    }

    // Получение сгенерованного id визита
    int visitId;
    QSqlQuery visitIdQuery("SELECT last_insert_rowid()");
    visitIdQuery.next();
    visitId = visitIdQuery.value(0).toInt();

    // Сохранение диагнозов
    QSqlQuery diagnInsQuery;
    diagnInsQuery.prepare(QString("INSERT INTO visit_diagnosis (visit_id, diagnosis_id)")
                          + QString("VALUES (:visit_id, :diagnosis_id)"));
    QMapIterator<int, QString> i(*emplDiagnLstModel->getDiagnosises());
    while (i.hasNext())
    {
        i.next();
        diagnInsQuery.bindValue(":visit_id", visitId);
        diagnInsQuery.bindValue(":diagnosis_id", i.key());
        diagnInsQuery.exec();
    }

    QSqlQuery endTrans;
    endTrans.exec("END TRANSACTION");

    return;
}

void EmployeeVisitDialog::updateVisit()
{
    //--*- Сначала проверка ошибок ввода.
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
    int id = visitToEdit->getId();
    QLocale locale(QLocale::Russian, QLocale::RussianFederation);
    float uet = locale.toFloat(ui->uetEdit->text());

    // Дата посещения
    QDate date = ui->visitDateDEdit->date();

    //--*- Затем сохранение в БД отредактированного визита
    QSqlQuery beginTrans;
    beginTrans.exec("BEGIN TRANSACTION");

    QSqlQuery query;
    query.prepare(QString("UPDATE visit SET employee_id = :employee_id, ")
                  + QString("visit_date = :visit_date, uet = :uet ")
                  + QString("WHERE id = :id"));
    query.bindValue(":id", id);
    query.bindValue(":employee_id", employeeId);
    query.bindValue(":visit_date", date);
    query.bindValue(":uet", uet);

    if(!query.exec()){
        QMessageBox::critical(this, tr("Ошибка сохранения в БД"),
                              query.lastError().text());
        qDebug() << "== Error ======================";
        qDebug() << "Number:        " << query.lastError().number();
        qDebug() << "Type:          " << query.lastError().type();
        qDebug() << "Text:          " << query.lastError().text();
        qDebug() << "Datebase Text: " << query.lastError().databaseText();


    }

    //--*- Теперь удаление из БД старих диагнозов,
    //     отсутствующих в новом списке.
    QListIterator<DiagnosisAR> oldDI (visitToEdit->getDiagnosises());
    QMapIterator<int, QString> newDI(*emplDiagnLstModel->getDiagnosises());
    bool exists;
    while( oldDI.hasNext() ) {
        DiagnosisAR oldDiagnosis = oldDI.next();
        exists = false;
        newDI.toFront();
        while( newDI.hasNext() )
        {
            newDI.next();
            if(newDI.key() == oldDiagnosis.getId()) {
                exists = true;
                //break;
            }
        }
        if (!exists) {
            QSqlQuery delOldQ;
            delOldQ.exec(QString("DELETE FROM visit_diagnosis ")
                         + QString("WHERE visit_id = %1 AND diagnosis_id = %2")
                         .arg(visitToEdit->getId())
                         .arg(oldDiagnosis.getId()));
        }
    }

    //--*- Последнее - добавление в БД диагнозов нового списка,
    //     отсутствующих в старом списке.
    newDI.toFront();
    while( newDI.hasNext() ) {
        newDI.next();
        exists = false;
        oldDI.toFront();
        while( oldDI.hasNext() ) {

            DiagnosisAR oldDiagnosis = oldDI.next();
            if( newDI.key() == oldDiagnosis.getId() )
            {
                exists = true;
            }
        }
        if( !exists )
        {
            QSqlQuery addNewQ;
            addNewQ.prepare(QString("INSERT INTO visit_diagnosis (visit_id, diagnosis_id) ")
                          + QString("VALUES (:visit_id, :diagnosis_id)"));
            addNewQ.bindValue( ":visit_id", visitToEdit->getId() );
            addNewQ.bindValue( ":diagnosis_id", newDI.key() );
            addNewQ.exec();
            // TODO: Обработать ошибки вставки
        }
    }

    QSqlQuery endTrans;
    endTrans.exec("END TRANSACTION");

    return;
}

void EmployeeVisitDialog::on_emplLineEdit_returnPressed()
{
    ui->emplListView->setFocus();
    ui->emplListView->selectionModel()->clear();
    ui->emplListView->selectionModel()->setCurrentIndex(emplLstModel->index(0, 1),
                                                        QItemSelectionModel::Select);
}

