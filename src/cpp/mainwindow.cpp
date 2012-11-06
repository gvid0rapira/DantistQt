#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/h/db/dbstructure.h"

#include <QtSql>
#include <QIcon>
#include <QGridLayout>
#include <QMessageBox>

#include <QModelIndexList>
#include <QModelIndex>

#include <QFile>
#include <QFileDialog>

#include <QAxObject>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Обнуляем диалоги, т.к. почему-то, после объявления, указатели на
    // диалоги не равны 0.
    this->employeeDlg = 0;
    this->employeeVisitDlg = 0;

    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();

    // Панель списка сотрудников
    employeeLstModel = new QSqlTableModel(this);
    employeeLstModel->setTable("employee");
    employeeLstModel->setHeaderData(Employee_Lname, Qt::Horizontal, tr("Фамилия"));
    employeeLstModel->setHeaderData(Employee_Fname, Qt::Horizontal, tr("Имя"));
    employeeLstModel->setHeaderData(Employee_Mname, Qt::Horizontal, tr("Отчество"));
    employeeLstModel->setHeaderData(Employee_Tab_Num, Qt::Horizontal, tr("Таб. №"));
    employeeLstModel->setHeaderData(Employee_Work_Place, Qt::Horizontal,
                                    tr("Подразделение"));
    employeeLstModel->select();

    employeeLstView = new QTableView;
    employeeLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeLstView->setModel(employeeLstModel);
    employeeLstView->setColumnHidden(Employee_Id, true);
    employeeLstView->resizeColumnsToContents();

    // Панель списка визитов
    visitFilterFrom = new QDateEdit(this);
    visitFilterFrom->setMaximumWidth(80);
    visitFilterFrom->setDate(QDate(QDate::currentDate().year(),
                                   QDate::currentDate().month(),
                                   1));
    visitFilterTo = new QDateEdit(this);
    visitFilterTo->setMaximumWidth(80);
    visitFilterTo->setDate(QDate(QDate::currentDate().year(),
                                 QDate::currentDate().month(),
                                 QDate::currentDate().daysInMonth()));

    visitFilterApplyBtn = new QPushButton(tr("Обновить") ,this);
    visitFilterApplyBtn->setMaximumWidth(80);
    connect(visitFilterApplyBtn, SIGNAL(clicked()), this, SLOT(updateVisitsLst()));

    QHBoxLayout *visitFilterLayout = new QHBoxLayout(this);
    visitFilterLayout->addWidget(visitFilterFrom);
    visitFilterLayout->addWidget(visitFilterTo);
    visitFilterLayout->addWidget(visitFilterApplyBtn);
    visitFilterLayout->addStretch(500);

    QWidget *visitFilterPanel = new QWidget(this);
    visitFilterPanel->setLayout(visitFilterLayout);

    employeeVisitLstModel = new QSqlQueryModel(this);
    employeeVisitLstModel->setQuery(getEmplVisitQuery(
                                        visitFilterFrom->date(),
                                        visitFilterTo->date()));
    employeeVisitLstModel->setHeaderData(1, Qt::Horizontal,
                                         tr("Фамилия"));
    employeeVisitLstModel->setHeaderData(2, Qt::Horizontal,
                                         tr("Имя"));
    employeeVisitLstModel->setHeaderData(3, Qt::Horizontal,
                                         tr("Отчество"));
    employeeVisitLstModel->setHeaderData(4, Qt::Horizontal,
                                         tr("Таб. №"));
    employeeVisitLstModel->setHeaderData(5, Qt::Horizontal,
                                         tr("Дата посещения"));

    employeeVisitLstView = new QTableView;
    employeeVisitLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeVisitLstView->setModel(employeeVisitLstModel);
    employeeVisitLstView->setColumnHidden(0, true);
    employeeVisitLstView->resizeColumnsToContents();

    QWidget *visitLstPanel = new QWidget(this);
    QVBoxLayout *visitLstLayout = new QVBoxLayout(this);
    visitLstPanel->setLayout(visitLstLayout);

    visitLstLayout->addWidget(visitFilterPanel);
    visitLstLayout->addWidget(employeeVisitLstView);


    // Панель простого отчета
    reportSimplePanel   = new QWidget();
    repSimpleTitleLbl   = new QLabel(tr("Реестр"), this);
    repSimpleFromLbl    = new QLabel(tr("с"), this);
    repSimpleFromDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                             QDate::currentDate().month(),
                                             1), this);
    repSimpleToLbl      = new QLabel(tr("по"), this);
    repSimpleToDEdit   = new QDateEdit(QDate(QDate::currentDate().year(),
                                             QDate::currentDate().month(),
                                             QDate::currentDate().daysInMonth()), this);
    repSimpleMakeBtn    = new QPushButton(tr("Сформировать"));
    repSimpleMakeBtn->setMaximumWidth(100);
    connect(repSimpleMakeBtn, SIGNAL(clicked()), this, SLOT(makeReportSimple()));


    QGridLayout *repSimpleMainLayout = new QGridLayout;
    repSimpleMainLayout->addWidget(repSimpleTitleLbl, 0, 0, 1, 3, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleFromLbl, 1, 0, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleFromDEdit, 1, 1, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleToLbl, 2, 0, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleToDEdit, 2, 1, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleMakeBtn, 1, 2, 1, 2, Qt::AlignTop);


    repSimpleMainLayout->setRowStretch(2, 500);
    repSimpleMainLayout->setColumnStretch(2, 500);

    reportSimplePanel->setLayout(repSimpleMainLayout);

    // Панель отчета для бухгалтерии
    repAccPanel = new QWidget();
    repAccTitleLbl = new QLabel(tr("Отчет для бухгалтерии"), this);
    repAccFromLbl = new QLabel(tr("с"), this);
    repAccFromDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                          QDate::currentDate().month(),
                                          1), this);
    repAccToLbl = new QLabel(tr("по"), this);
    repAccToDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                        QDate::currentDate().month(),
                                        QDate::currentDate().daysInMonth()), this);
    repAccMakeBtn =  new QPushButton(tr("Сформировать"));
    repAccMakeBtn->setMaximumWidth(100);
    connect(repAccMakeBtn, SIGNAL(clicked()), this, SLOT(makeReportAccounting()));

    QGridLayout *repAccMainLayout = new QGridLayout;
    repAccMainLayout->addWidget(repAccTitleLbl, 0, 0, 1, 3, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccFromLbl, 1, 0, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccFromDEdit, 1, 1, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccToLbl, 2, 0, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccToDEdit, 2, 1, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccMakeBtn, 1, 2, 1, 2, Qt::AlignTop);

    repAccMainLayout->setRowStretch(2, 500);
    repAccMainLayout->setColumnStretch(2, 500);

    repAccPanel->setLayout(repAccMainLayout);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(employeeLstView);
    stackedWidget->addWidget(visitLstPanel);
    stackedWidget->addWidget(reportSimplePanel);
    stackedWidget->addWidget(repAccPanel);

    stackedWidget->setCurrentIndex(1);

    this->setCentralWidget(stackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateVisitsLst()
{
    employeeVisitLstModel->setQuery(getEmplVisitQuery(
                                        visitFilterFrom->date(),
                                        visitFilterTo->date()));
}

void MainWindow::exit()
{
    qApp->exit();
}

void MainWindow::about()
{

}

void MainWindow::showVisits()
{
    employeeToolBar->hide();
    employeeVisitToolBar->show();
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showEmployees()
{
    employeeVisitToolBar->hide();
    employeeToolBar->show();
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showReportSimple()
{
    employeeVisitToolBar->hide();
    employeeToolBar->hide();
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::makeReportSimple()
{
    QDate from = repSimpleFromDEdit->date();
    QDate to   = repSimpleToDEdit->date();

    QString sql = QString("SELECT e.lname, e.fname, e.mname, v.visit_date ")
            + QString("FROM employee e, visit v ")
            + QString("WHERE (date(v.visit_date) BETWEEN date('%1') AND date('%2'))")
                .arg(from.toString("yyyy-MM-dd"))
                .arg(to.toString("yyyy-MM-dd"))
            + QString(" AND  v.employee_id = e.id");
    qDebug() << sql;
    QSqlQuery query(sql);
    if(query.isActive()) {
        query.finish();
    }
    query.exec();

    // Создание файла от чета из шаблона
    QString tmplFileName(QString(
                     #ifdef QT_DEBUG
                         "%1\\..\\DantistQt\\templates\\register.xlsx"
                     #else
                         "%1\\templates\\register.xlsx"
                     #endif
                         ).arg(QDir::currentPath()));
    QString regFileName(QString(
                     #ifdef QT_DEBUG
                        "%1\\..\\DantistQt\\reports\\register_%2.xlsx"
                     #else
                        "%1\\reports\\register_%2.xlsx"
                     #endif
                        ).arg(QDir::currentPath())
                        .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
    QFile templateFile(tmplFileName);
    templateFile.copy(regFileName);

    QAxObject *excel = new QAxObject( "Excel.Application", 0 );
    excel->setProperty("Visible", true);

    QAxObject *workbooks = excel->querySubObject( "WorkBooks" );
    QAxObject *workbook  = workbooks->querySubObject(//"Add");
                "Open(const QString&", regFileName);

    // QAxObject *sheets = workbook->querySubObject( "Worksheets" );
    QAxObject *sheet = workbook->querySubObject("Worksheets(Int)", 1);
    // Запись периода отчета в отчет
    sheet->querySubObject("Cells(Int, Int)", 4, 2 )
            ->setProperty("Value", QVariant(QString(tr("За период с "))
                                            + from.toString("dd.MM.yyyy")
                                            + QString(tr(" по "))
                                            + to.toString("dd.MM.yyyy")));

    int startRow = 7;
    int count = 0;
    while( query.next() ) {
        QString lname = query.value(0).toString();
        QString fname = query.value(1).toString();
        QString mname = query.value(2).toString();
        QDate visiDate = query.value(3).toDate();
        qDebug() << lname << fname << mname;
        count++;
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 1 )
                ->setProperty("Value", QVariant(count));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 2 )
                ->setProperty("Value", QVariant(lname + " " + fname + " " + mname));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 4 )
                ->setProperty("Value", QVariant(visiDate.toString("dd.MM.yyyy")));

        // Рисование границ
        drawCellBorders(*sheet, startRow + count, 5);
    }

    sheet->querySubObject("Cells(Int, Int)", startRow + count + 2, 2 )
            ->setProperty("Value", QVariant(tr("Исп. Панкратова А.В.")));
    sheet->querySubObject("Cells(Int, Int)", startRow + count + 3, 2 )
            ->setProperty("Value", QVariant(tr("Тел. 47-5-25")));

    QAxObject *cell = sheet->querySubObject("Cells(Int, Int)", 3, 2 );
    QVariant cellValue = cell->dynamicCall("value");
    cell->dynamicCall("Value");
    qDebug() << cellValue;


    workbook->dynamicCall("Save()");
    //    excel->dynamicCall("Quit()");

}

void MainWindow::showReportAccounting(){
    employeeVisitToolBar->hide();
    employeeToolBar->hide();
    stackedWidget->setCurrentIndex(3);}

void MainWindow::makeReportAccounting(){
    QDate from = repAccFromDEdit->date();
    QDate to   = repAccToDEdit->date();

    // TODO: вынести константу 250 в конфигурационный файл
    QString sql = QString("SELECT DISTINCT e.lname, e.fname, e.mname, e.tab_num, e.work_place, ")
            + QString("v.visit_date, count(e.id), count(e.id)*750 ")
            + QString("FROM employee e, visit v ")
            + QString("WHERE (v.visit_date BETWEEN date('%1') AND date('%2')) ")
                .arg(from.toString("yyyy-MM-dd"))
                .arg(to.toString("yyyy-MM-dd"))
            + QString("AND v.employee_id = e.id ")
            + QString("GROUP BY e.lname");
    qDebug() << sql;

    QSqlQuery query(sql);
    if(!query.exec()) {
        qDebug() << tr("makeReportAccounting error: %1").arg(query.lastError().text());
    }

    // Создание файла отчета из шаблона

    QString tmplFileName(QString(
                       #ifdef QT_DEBUG
                           "%1\\..\\DantistQt\\templates\\accounting_report.xlsx"
                       #else
                           "%1\\templates\\accounting_report.xlsx"
                       #endif
                           ).arg(QDir::currentPath()));
    QString accRepFileName(QString(
                       #ifdef QT_DEBUG
                           "%1\\..\\DantistQt\\reports\\accounting_report_%2.xlsx"
                       #else
                           "%1\\reports\\accounting_report_%2.xlsx"
                       #endif
                           ).arg(QDir::currentPath())
                            .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
    QFile templateFile(tmplFileName);
    templateFile.copy(accRepFileName);

    QAxObject *excel = new QAxObject( "Excel.Application", 0 );
    excel->setProperty("Visible", true);

    QAxObject *workbooks = excel->querySubObject( "WorkBooks" );
    QAxObject *workbook  = workbooks->querySubObject(//"Add");
                "Open(const QString&", accRepFileName);

    QAxObject *sheet = workbook->querySubObject("Worksheets(Int)", 1);
    //Формирование шапки отчета
    sheet->querySubObject("Cells(Int, Int)", 5, 5 )
            ->setProperty("Value", QVariant(tr("''____''___________ %1г.")
                                            .arg(QDate::currentDate().year())));
    sheet->querySubObject("Cells(Int, Int)", 9, 4 )
            ->setProperty("Value", QVariant(QString(tr("с "))
                                            + from.toString("dd.MM.yyyy")
                                            + QString(tr(" по "))
                                            + to.toString("dd.MM.yyyy")));

    int startRow = 12;
    int rowCount = 0;
    int allVisits = 0;
    int totalSpent =0;
    while( query.next() ) {
        QString lname = query.value(0).toString();
        QString fname = query.value(1).toString();
        QString mname = query.value(2).toString();
        int tab_num   = query.value(3).toInt();
        QString work_place = query.value(4).toString();
        int visit_count = query.value(6).toInt();
        allVisits += visit_count;
        int sum_spent = query.value(7).toInt();
        totalSpent += sum_spent;

        qDebug() << lname << " " << fname << " " << mname << ", "
                 << tab_num << ", " << work_place << ", " << visit_count << ", " << sum_spent;

        rowCount++;
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 1 )
                ->setProperty("Value", QVariant(rowCount));
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 2 )
                ->setProperty("Value", QVariant(lname + " " + fname + " " + mname));
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 3 )
                ->setProperty("Value", QVariant(tab_num));
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 4 )
                ->setProperty("Value", QVariant(work_place));
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 5 )
                ->setProperty("Value", QVariant(visit_count));
        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 6 )
                ->setProperty("Value", QVariant(sum_spent));
        drawCellBorders(*sheet, startRow + rowCount, 7);
    }

    // Подошва отчета
    drawCellBorders(*sheet, startRow + rowCount + 1, 7);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("HorizontalAlignment", -4152);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("Value", QVariant(tr("Итого:")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 5 )
            ->setProperty("Value", allVisits);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 6 )
            ->setProperty("Value", totalSpent);

    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 2 )
            ->setProperty("HorizontalAlignment", -4152);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 2 )
            ->setProperty("Value", QVariant(tr("Зубной врач")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 6 )
            ->setProperty("Value", QVariant(tr("Панкратова А.В.")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 3 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 4 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 5 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);

    workbook->dynamicCall("Save()");

}

void MainWindow::drawCellBorders(QAxObject &sheet, int row, int colCount)
{
    for(int i = 1; i <= colCount; i++) {
        sheet.querySubObject("Cells(Int, Int)", row, i )
                ->querySubObject("Borders(Int)", 7)
                ->setProperty("LineStyle", 1);
        sheet.querySubObject("Cells(Int, Int)", row, i )
                ->querySubObject("Borders(Int)", 9)
                ->setProperty("LineStyle", 1);
        sheet.querySubObject("Cells(Int, Int)", row, i )
                ->querySubObject("Borders(Int)", 10)
                ->setProperty("LineStyle", 1);
    }
}

void MainWindow::importEmplCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Открыть CSV файл"), QDir::currentPath(),
                   tr("Файл CSV (разделитель точка с запятой) (*.csv)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << tr("Ошибка открытия файла %1").arg(fileName).toStdString()
                  << std::endl;
        return;
    }


    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("Windows-1251"));
    QSqlQuery delQuery("DELETE FROM employee");
    delQuery.exec();
    QSqlQuery insQuery;
    insQuery.prepare(QString("INSERT INTO employee ")
                  + QString("(lname, fname, mname, tab_num, work_place) ")
                  + QString("VALUES ")
                  + QString("(:lname, :fname, :mname, :tab_num, :work_place)"));
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(';');
        if (fields.size() >= 5) {
            int tab_num = fields.takeFirst().toInt();
            QString lname = fields.takeFirst().trimmed().toLower();
            lname = lname.left(1).toUpper() + lname.right(lname.length() - 1);
            QString fname = fields.takeFirst().trimmed().toLower();
            fname = fname.left(1).toUpper() + fname.right(fname.length() - 1);
            QString mname = fields.takeFirst().trimmed().toLower();
            mname = mname.left(1).toUpper() + mname.right(mname.length() - 1);
            QString work_place = fields.takeFirst();
            insQuery.bindValue(":lname", lname);
            insQuery.bindValue(":fname", fname);
            insQuery.bindValue(":mname", mname);
            insQuery.bindValue(":tab_num", tab_num);
            insQuery.bindValue(":work_place", work_place);
            if(!insQuery.exec()){

                qDebug() << "== Error ======================";
                qDebug() << "Number:        " << insQuery.lastError().number();
                qDebug() << "Type:          " << insQuery.lastError().type();
                qDebug() << "Text:          " << insQuery.lastError().text();
                qDebug() << "Datebase Text: " << insQuery.lastError().databaseText();


            }

        }
    }
    file.close();
    QMessageBox msg(this);
    msg.setText(tr("Импорт закончен."));
    msg.exec();
}

void MainWindow::addEmployee()
{
    if(!employeeDlg){
        employeeDlg = new EmployeeDialog(-1, employeeLstModel, this);
    } else {
        employeeDlg->rowToEdit(-1);
    }
    employeeDlg->show();
    employeeDlg->raise();
    employeeDlg->activateWindow();
}

void MainWindow::editEmployee()
{
    QModelIndexList list = employeeLstView->selectionModel()->selection().indexes();
    QModelIndex index = list.first();
    int row = index.row();
    if(!employeeDlg){
        employeeDlg = new EmployeeDialog(row, employeeLstModel, this);
    } else {
        employeeDlg->rowToEdit(row);
    }
    employeeDlg->show();
    employeeDlg->raise();
    employeeDlg->activateWindow();
}

void MainWindow::delEmployee()
{
    QModelIndexList list = employeeLstView->selectionModel()->selection().indexes();
    QModelIndex index = list.first();
    int row = index.row();
    employeeLstModel->removeRow(row);
    employeeLstModel->submitAll();
}

void MainWindow::addEmployeeVisit()
{
    if(!employeeVisitDlg){
        employeeVisitDlg = new EmployeeVisitDialog(-1, employeeVisitLstModel, this);
        connect(employeeVisitDlg, SIGNAL(visitInserted()), this, SLOT(updateVisitsLst()));
        connect(addEmployeeVisitAction, SIGNAL(triggered()), employeeVisitDlg, SLOT(initBeforDisplay()));
    } else {
        employeeVisitDlg->rowToEdit(-1);
    }
    employeeVisitDlg->show();
    employeeVisitDlg->raise();
    employeeVisitDlg->activateWindow();
}

void MainWindow::editEmployeeVisit()
{
    QModelIndexList list = employeeVisitLstView->selectionModel()->selection().indexes();
    QModelIndex index = list.first();
    int row = index.row();
    if(!employeeVisitDlg){
        employeeVisitDlg = new EmployeeVisitDialog(row, employeeVisitLstModel, this);
    } else {
        employeeVisitDlg->rowToEdit(row);
    }
    employeeVisitDlg->show();
    employeeVisitDlg->raise();
    employeeVisitDlg->activateWindow();
}
void MainWindow::delEmployeeVisit(){
    QModelIndexList list = employeeVisitLstView->selectionModel()->selection().indexes();
    QModelIndex index = list.first();
    int row = index.row();
    int selectId = employeeVisitLstModel->index(row, 0).data().toInt();
    employeeVisitLstModel->removeRow(row);
    QString sql = QString("DELETE FROM visit WHERE id = %1").arg(selectId);
    QSqlQuery qury(sql);

    updateVisitsLst();
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("Выход"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    aboutAction = new QAction(tr("О программе"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    showVisitsAction = new QAction(tr("Визиты"), this);
    connect(showVisitsAction, SIGNAL(triggered()), this, SLOT(showVisits()));

    showEmployeesAction = new QAction(tr("Сотрудники"), this);
    connect(showEmployeesAction, SIGNAL(triggered()), this, SLOT(showEmployees()));

    reportSimpleAction = new QAction(tr("Реестр"), this);
    connect(reportSimpleAction, SIGNAL(triggered()), this, SLOT(showReportSimple()));

    reportForAccountingAction = new QAction(tr("Для бухгалтерии"), this);
    connect(reportForAccountingAction, SIGNAL(triggered()),
            this, SLOT(showReportAccounting()));

    importEmplCSVAction = new QAction(tr("Импорт сотрудников из CSV"), this);
    connect(importEmplCSVAction, SIGNAL(triggered()), this, SLOT(importEmplCSV()));

    addEmployeeAction = new QAction(tr("Добавить сотрудника"), this);
    addEmployeeAction->setIcon(QIcon(":/images/add_file.png"));
    addEmployeeAction->setToolTip(tr("Добавить сотрудника"));
    addEmployeeAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(addEmployeeAction, SIGNAL(triggered()), this, SLOT(addEmployee()));

    editEmployeeAction = new QAction(tr("Редактировать сотрудника"), this);
    editEmployeeAction->setIcon(QIcon(":/images/edit.png"));
    editEmployeeAction->setToolTip(tr("Редактировать сотрудника"));
    connect(editEmployeeAction, SIGNAL(triggered()), this, SLOT(editEmployee()));

    delEmployeeAction = new QAction(tr("Удалить сотрудника"), this);
    delEmployeeAction->setIcon(QIcon(":/images/delete.png"));
    delEmployeeAction->setToolTip(tr("Удалить сотрудника"));
    connect(delEmployeeAction, SIGNAL(triggered()), this, SLOT(delEmployee()));

    addEmployeeVisitAction = new QAction(tr("Добавить визит сотрудника"), this);
    addEmployeeVisitAction->setIcon(QIcon(":/images/add_file.png"));
    addEmployeeVisitAction->setToolTip(tr("Добавить визит сотрудника"));
    addEmployeeVisitAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(addEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(addEmployeeVisit()));

    editEmployeeVisitAction = new QAction(tr("Редактировать визит сотрудника"), this);
    editEmployeeVisitAction->setIcon(QIcon(":/images/edit.png"));
    editEmployeeVisitAction->setToolTip(tr("Редактировать визит сотрудника"));
    connect(editEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(editEmployeeVisit()));

    delEmployeeVisitAction = new QAction(tr("Удалить визит сотрудника"), this);
    delEmployeeVisitAction->setIcon(QIcon(":/images/delete.png"));
    delEmployeeVisitAction->setToolTip(tr("Удалить визит сотрудника"));
    connect(delEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(delEmployeeVisit()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(exitAction);

    listsMenu = menuBar()->addMenu(tr("Списки"));
    listsMenu->addAction(showVisitsAction);
    listsMenu->addAction(showEmployeesAction);

    reportsMenu = menuBar()->addMenu(tr("Отчеты"));
    reportsMenu->addAction(reportSimpleAction);
    reportsMenu->addAction(reportForAccountingAction);

    systemMenu = menuBar()->addMenu(tr("Системное"));
    systemMenu->addAction(importEmplCSVAction);

    helpMenu = menuBar()->addMenu(tr("Помощь"));
    helpMenu->addAction(aboutAction);

}

void MainWindow::createToolBars()
{
    employeeToolBar = addToolBar(tr("Панель сотрудника"));
    employeeToolBar->addAction(addEmployeeAction);
    employeeToolBar->addAction(editEmployeeAction);
    employeeToolBar->addAction(delEmployeeAction);
    employeeToolBar->hide();

    employeeVisitToolBar = addToolBar(tr("Панель визита сотрудника"));
    employeeVisitToolBar->addAction(addEmployeeVisitAction);
    // employeeVisitToolBar->addAction(editEmployeeVisitAction);
    employeeVisitToolBar->addAction(delEmployeeVisitAction);

}

