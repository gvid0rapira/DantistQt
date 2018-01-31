#include "src/h/diagnosisdialog.h"
#include "ui_diagnosisdialog.h"

#include <QtSql>
#include <QMessageBox>

DiagnosisDialog::DiagnosisDialog(QSqlTableModel *m, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagnosisDialog)
{
    ui->setupUi(this);
    model = m;

    //--*- Отображение модели данных на виджеты
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->addMapping(ui->nameEdt, 1);
}

DiagnosisDialog::~DiagnosisDialog()
{
    delete ui;
}

void DiagnosisDialog::on_cancelBtn_clicked()
{
    model->removeRow(mapper->currentIndex());
    this->hide();
}

void DiagnosisDialog::on_OKBtn_clicked()
{
    QString errStr;
    if(ui->nameEdt->text().length() == 0)
        errStr.append(tr("  * Название\n"));
    if(errStr.length() != 0) {
        errStr.insert(0, tr("Необходимо заполнить поля: \n"));
        QMessageBox::warning(this, tr("Ошибка ввода"), errStr);
        return;
    }
    if(!mapper->submit()) {
        QMessageBox::critical(this, tr("Ошибка записи в БД"), model->lastError().databaseText());
        return;
    }
    this->hide();

    return;
}

void DiagnosisDialog::initEditMode(int row)
{
    if(row != -1)
    {
        mode = FORM_MODE_EDIT;
        mapper->setCurrentIndex(row);
    } else {
        mode = FORM_MODE_ADD;
        mapper->toLast();
        int row = mapper->currentIndex();
        if(row == -1) row = 0; // Если модель пуста
        model->insertRow(row);
        mapper->setCurrentIndex(row);
    }
}

