#include <src/h/employeedialog.h>
#include <QGridLayout>
#include <QHBoxLayout>
#include <src/h/db/dbstructure.h>

#include <QRegExpValidator>
#include <QMessageBox>
#include <QtSql>

EmployeeDialog::EmployeeDialog(int row, QSqlTableModel *m, QWidget *parent)
    : QDialog(parent)
{
    model = m;

    //--*- Формируем интерфейс

    QRegExpValidator *nameValidator
            = new QRegExpValidator(QRegExp(tr("[А-Я][а-я]{1,50}")) ,this);
    lnameLabel = new QLabel(tr("Фамилия:"), this);
    lnameEdit = new QLineEdit(this);
    lnameEdit->setValidator(nameValidator);
    lnameLabel->setBuddy(lnameEdit);

    fnameLabel = new QLabel(tr("Имя:"));
    fnameEdit = new QLineEdit;
    fnameEdit->setValidator(nameValidator);
    fnameLabel->setBuddy(fnameEdit);

    mnameLabel = new QLabel(tr("Отчество:"));
    mnameEdit = new QLineEdit;
    mnameEdit->setValidator(nameValidator);
    mnameLabel->setBuddy(mnameEdit);

    tabNumLabel = new QLabel(tr("Таб.№:"));
    tabNumEdit = new QLineEdit;
    tabNumEdit->setValidator(new QIntValidator(0, 1000000, this));
    tabNumLabel->setBuddy(tabNumEdit);

    depLabel = new QLabel(tr("Подразделение:"));
    depEdit = new QLineEdit;
    depLabel->setBuddy(depEdit);

    acceptButton = new QPushButton(tr("OK"));
    rejectButton = new QPushButton(tr("Отмена"));
    rejectButton->setDefault(true);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptInput()));//SLOT(accept()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(rejectInput()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(rejectButton);
    buttonLayout->setAlignment(Qt::AlignRight);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(lnameLabel, 0, 0);
    mainLayout->addWidget(lnameEdit, 0, 1);
    mainLayout->addWidget(fnameLabel, 1, 0);
    mainLayout->addWidget(fnameEdit, 1, 1);
    mainLayout->addWidget(mnameLabel, 2, 0);
    mainLayout->addWidget(mnameEdit, 2, 1);
    mainLayout->addWidget(tabNumLabel, 3, 0);
    mainLayout->addWidget(tabNumEdit, 3, 1);
    mainLayout->addWidget(depLabel, 4, 0);
    mainLayout->addWidget(depEdit, 4, 1);
    mainLayout->addLayout(buttonLayout, 5, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("Сотрудник"));

    //--*- Отображение модели данных на виджеты
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    mapper->addMapping(lnameEdit, Employee_Lname);
    mapper->addMapping(fnameEdit, Employee_Fname);
    mapper->addMapping(mnameEdit, Employee_Mname);
    mapper->addMapping(depEdit, Employee_Work_Place);
    mapper->addMapping(tabNumEdit, Employee_Tab_Num);

    rowToEdit(row);
}

EmployeeDialog::~EmployeeDialog()
{

}

void EmployeeDialog::acceptInput()
{
    QString errStr;
    if(lnameEdit->text().length() == 0)
        errStr.append(tr("  * Фамилия\n"));
    if(fnameEdit->text().length() == 0)
        errStr.append(tr("  * Имя\n"));

    if(tabNumEdit->text().length() == 0)
        errStr.append(tr("  * Табельный номер\n"));
    if(depEdit->text().length() == 0)
        errStr.append(tr("  * Подразделение \n"));
    if(errStr.length() != 0) {
        errStr.insert(0, tr("Необходимо заполнить поля: \n"));
        QMessageBox::warning(this, tr("Ошибка ввода"), errStr);
        return;
    }
    if(!mapper->submit()) {
        QMessageBox::critical(this, "Ошибка записи в БД", model->lastError().databaseText());
        return;
    }
    this->hide();

    return;
}

void EmployeeDialog::rejectInput()
{
    if(mode == FORM_MODE_ADD) {
        model->removeRow(mapper->currentIndex());
    }
    this->hide();

    return;
}

void EmployeeDialog::rowToEdit(int row)
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
