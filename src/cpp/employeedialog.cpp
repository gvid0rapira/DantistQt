#include <src/h/employeedialog.h>
#include <QGridLayout>
#include <QHBoxLayout>


EmployeeDialog::EmployeeDialog(int row, QSqlTableModel *m, QWidget *parent)
    : QDialog(parent)
{
    model = m;

    //--*- Формируем интерфейс
    lnameLabel = new QLabel(tr("Фамилия:"));
    lnameEdit = new QLineEdit;
    lnameLabel->setBuddy(lnameEdit);

    fnameLabel = new QLabel(tr("Имя:"));
    fnameEdit = new QLineEdit;
    fnameLabel->setBuddy(fnameEdit);

    mnameLabel = new QLabel(tr("Отчество:"));
    mnameEdit = new QLineEdit;
    mnameLabel->setBuddy(mnameEdit);

    tabNumLabel = new QLabel(tr("Таб.№:"));
    tabNumEdit = new QLineEdit;
    tabNumLabel->setBuddy(tabNumEdit);

    depLabel = new QLabel(tr("Подразделение:"));
    depEdit = new QLineEdit;
    depLabel->setBuddy(depEdit);

    acceptButton = new QPushButton(tr("OK"));
    rejectButton = new QPushButton(tr("Отмена"));
    rejectButton->setDefault(true);

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(acceptInput()));//SLOT(accept()));
    connect(rejectButton, SIGNAL(clicked()), this, SLOT(reject()));

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
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(model);
    mapper->addMapping(lnameEdit, Employee_Lname);
    mapper->addMapping(fnameEdit, Employee_Fname);
    mapper->addMapping(mnameEdit, Employee_Mname);
    mapper->addMapping(depEdit, Employee_Dep);
    mapper->addMapping(tabNumEdit, Employee_TabNumber);

    if(row != -1)
    {
        mapper->setCurrentIndex(row);
    } else {

        mapper->toLast();
        int row = mapper->currentIndex();
        model->insertRow(row);
        mapper->setCurrentIndex(row);
    }
}

EmployeeDialog::~EmployeeDialog()
{

}

void EmployeeDialog::acceptInput()
{
    model->submitAll();
    this->hide();
}

void EmployeeDialog::rowToEdit(int row)
{
    mapper->setCurrentIndex(row);
}
