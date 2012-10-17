#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/h/db/dbstructure.h"

#include <QtSql>
#include <QIcon>
#include <QGridLayout>
#include <QDateTimeEdit>
#include <QMessageBox>

#include <QModelIndexList>
#include <QModelIndex>

#include <QFile>
#include <QFileDialog>

#include <QAxObject>

#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // �������� �������, �.�. ������-��, ����� ����������, ��������� ��
    // ������� �� ����� 0.
    this->employeeDlg = 0;
    this->employeeVisitDlg = 0;

    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();

    // ������ ������ �����������
    employeeLstModel = new QSqlTableModel(this);
    employeeLstModel->setTable("employee");
    employeeLstModel->setHeaderData(Employee_Lname, Qt::Horizontal, tr("�������"));
    employeeLstModel->setHeaderData(Employee_Fname, Qt::Horizontal, tr("���"));
    employeeLstModel->setHeaderData(Employee_Mname, Qt::Horizontal, tr("��������"));
    employeeLstModel->setHeaderData(Employee_Tab_Num, Qt::Horizontal, tr("���. �"));
    employeeLstModel->setHeaderData(Employee_Work_Place, Qt::Horizontal,
                                    tr("�������������"));
    employeeLstModel->select();

    employeeLstView = new QTableView;
    employeeLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeLstView->setModel(employeeLstModel);
    employeeLstView->setColumnHidden(Employee_Id, true);
    employeeLstView->resizeColumnsToContents();

    // ������ ������ �������
    employeeVisitLstModel = new QSqlQueryModel(this);
    /*visitLstSql =
                 QString("SELECT v.id, e.lname, e.fname, e.mname, e.tab_num, v.visit_time ")
               + QString("FROM employee e, visit v ")
               + QString("WHERE v.employee_id = e.id");*/
    employeeVisitLstModel->setQuery(getEmplVisitSql());

    employeeVisitLstModel->setHeaderData(1, Qt::Horizontal,
                                         tr("�������"));
    employeeVisitLstModel->setHeaderData(2, Qt::Horizontal,
                                         tr("���"));
    employeeVisitLstModel->setHeaderData(3, Qt::Horizontal,
                                         tr("��������"));
    employeeVisitLstModel->setHeaderData(4, Qt::Horizontal,
                                         tr("���. �"));
    employeeVisitLstModel->setHeaderData(5, Qt::Horizontal,
                                         tr("����� ���������"));

    employeeVisitLstView = new QTableView;
    employeeVisitLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeVisitLstView->setModel(employeeVisitLstModel);
    employeeVisitLstView->setColumnHidden(0, true);
    employeeVisitLstView->resizeColumnsToContents();

    // ������ �������� ������
    reportSimplePanel   = new QWidget();
    repSimpleTitleLbl   = new QLabel(tr("������"), this);
    repSimpleFromLbl    = new QLabel(tr("�"), this);
    repSimpleFromDTEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    repSimpleToLbl      = new QLabel(tr("��"), this);
    repSimpleToDTEdit   = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    repSimpleMakeBtn    = new QPushButton(tr("������������"));
    repSimpleMakeBtn->setMaximumWidth(100);
    connect(repSimpleMakeBtn, SIGNAL(clicked()), this, SLOT(makeReportSimple()));


    QGridLayout *repSimpleMainLayout = new QGridLayout;
    repSimpleMainLayout->addWidget(repSimpleTitleLbl, 0, 0, 1, 3, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleFromLbl, 1, 0, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleFromDTEdit, 1, 1, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleToLbl, 2, 0, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleToDTEdit, 2, 1, 1, 1, Qt::AlignTop);
    repSimpleMainLayout->addWidget(repSimpleMakeBtn, 1, 2, 1, 2, Qt::AlignTop);


    repSimpleMainLayout->setRowStretch(2, 500);
    repSimpleMainLayout->setColumnStretch(2, 500);

    reportSimplePanel->setLayout(repSimpleMainLayout);

    // ������ ������ ��� �����������
    repAccPanel = new QWidget();
    repAccTitleLbl = new QLabel(tr("����� ��� �����������"), this);
    repAccFromLbl = new QLabel(tr("�"), this);
    repAccToDTEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    repAccToLbl = new QLabel(tr("��"), this);
    repAccFromDTEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    repAccMakeBtn =  new QPushButton(tr("������������"));
    repAccMakeBtn->setMaximumWidth(100);
    connect(repAccMakeBtn, SIGNAL(clicked()), this, SLOT(makeReportAccounting()));

    QGridLayout *repAccMainLayout = new QGridLayout;
    repAccMainLayout->addWidget(repAccTitleLbl, 0, 0, 1, 3, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccFromLbl, 1, 0, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccFromDTEdit, 1, 1, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccToLbl, 2, 0, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccToDTEdit, 2, 1, 1, 1, Qt::AlignTop);
    repAccMainLayout->addWidget(repAccMakeBtn, 1, 2, 1, 2, Qt::AlignTop);

    repAccMainLayout->setRowStretch(2, 500);
    repAccMainLayout->setColumnStretch(2, 500);

    repAccPanel->setLayout(repAccMainLayout);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(employeeLstView);
    stackedWidget->addWidget(employeeVisitLstView);
    stackedWidget->addWidget(reportSimplePanel);
    stackedWidget->addWidget(repAccPanel);

    stackedWidget->setCurrentIndex(1);

    this->setCentralWidget(stackedWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QDateTime from = repSimpleFromDTEdit->dateTime();
    QDateTime to   = repSimpleToDTEdit->dateTime();

    QString sql = QString("SELECT e.lname, e.fname, e.mname, v.visit_time ")
            + QString("FROM employee e, visit v ")
            + QString("WHERE (datetime(v.visit_time) BETWEEN datetime('%1') AND datetime('%2'))")
                .arg(from.toString("yyyy-MM-dd hh:mm"))
                .arg(to.toString("yyyy-MM-dd hh:mm"))
            + QString(" AND  v.employee_id = e.id");
    qDebug() << sql;
    QSqlQuery query(sql);
    if(query.isActive()) {
        query.finish();
    }
    query.exec();

    // �������� ����� �� ���� �� �������
    QString tmplFileName(QString("%1\\..\\DantistQt\\templates\\register.xlsx")
                             // "%1\\templates\\register.xlsx")
                         .arg(QDir::currentPath()));
    QString regFileName(QString("%1\\..\\DantistQt\\reports\\register_%2.xlsx")
                            // "%1\\reports\\register_%2.xlsx")
                        .arg(QDir::currentPath())
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
    // ������ ������� ������ � �����
    sheet->querySubObject("Cells(Int, Int)", 4, 2 )
            ->setProperty("Value", QVariant(QString(tr("�� ������ � "))
                                            + from.toString("dd.MM.yyyy")
                                            + QString(tr(" �� "))
                                            + to.toString("dd.MM.yyyy")));

    int startRow = 7;
    int count = 0;
    while( query.next() ) {
        QString lname = query.value(0).toString();
        QString fname = query.value(1).toString();
        QString mname = query.value(2).toString();
        QDateTime visitTime = query.value(3).toDateTime();
        qDebug() << lname << fname << mname;
        count++;
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 1 )
                ->setProperty("Value", QVariant(count));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 2 )
                ->setProperty("Value", QVariant(lname + " " + fname + " " + mname));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 4 )
                ->setProperty("Value", QVariant(visitTime.toString("dd.MM.yyyy")));

        // ��������� ������
        drawCellBorders(*sheet, startRow + count, 5);
    }

    sheet->querySubObject("Cells(Int, Int)", startRow + count + 2, 2 )
            ->setProperty("Value", QVariant(tr("���. ���������� �.�.")));
    sheet->querySubObject("Cells(Int, Int)", startRow + count + 3, 2 )
            ->setProperty("Value", QVariant(tr("���. 47-5-25")));

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
    QDateTime from = repAccFromDTEdit->dateTime();
    QDateTime to   = repAccToDTEdit->dateTime();

    // TODO: ������� ��������� 250 � ���������������� ����
    QString sql = QString("SELECT DISTINCT e.lname, e.fname, e.mname, e.tab_num, e.work_place, ")
            + QString("v.visit_time, count(e.id), count(e.id)*250 ")
            + QString("FROM employee e, visit v ")
            + QString("WHERE (v.visit_time BETWEEN datetime('%1') AND datetime('%2')) ")
                .arg(from.toString("yyyy-MM-dd hh:mm"))
                .arg(to.toString("yyyy-MM-dd hh:mm"))
            + QString("AND v.employee_id = e.id ")
            + QString("GROUP BY e.lname");
    qDebug() << sql;

    QSqlQuery query(sql);
    if(!query.exec()) {
        qDebug() << tr("makeReportAccounting error: %1").arg(query.lastError().text());
    }

    // �������� ����� ������ �� �������
    QString tmplFileName(QString(// "%1\\..\\DantistQt\\templates\\accounting_report.xlsx")
                             "%1\\templates\\accounting_report.xlsx")
                         .arg(QDir::currentPath()));
    QString accRepFileName(QString(// "%1\\..\\DantistQt\\reports\\accounting_report_%2.xlsx")
                               "%1\\reports\\accounting_report_%2.xlsx")
                         .arg(QDir::currentPath())
                         .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
    QFile templateFile(tmplFileName);
    templateFile.copy(accRepFileName);

    QAxObject *excel = new QAxObject( "Excel.Application", 0 );
    excel->setProperty("Visible", true);

    QAxObject *workbooks = excel->querySubObject( "WorkBooks" );
    QAxObject *workbook  = workbooks->querySubObject(//"Add");
                "Open(const QString&", accRepFileName);

    QAxObject *sheet = workbook->querySubObject("Worksheets(Int)", 1);
    //������������ ����� ������
    sheet->querySubObject("Cells(Int, Int)", 5, 5 )
            ->setProperty("Value", QVariant(tr("''____''___________ %1�.")
                                            .arg(QDate::currentDate().year())));
    sheet->querySubObject("Cells(Int, Int)", 9, 4 )
            ->setProperty("Value", QVariant(QString(tr("� "))
                                            + from.toString("dd.MM.yyyy")
                                            + QString(tr(" �� "))
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

    // ������� ������
    drawCellBorders(*sheet, startRow + rowCount + 1, 7);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("HorizontalAlignment", -4152);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("Value", QVariant(tr("�����:")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 5 )
            ->setProperty("Value", allVisits);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 6 )
            ->setProperty("Value", totalSpent);

    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 2 )
            ->setProperty("HorizontalAlignment", -4152);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 2 )
            ->setProperty("Value", QVariant(tr("������ ����")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 4, 6 )
            ->setProperty("Value", QVariant(tr("���������� �.�.")));
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
            tr("������� CSV ����"), QDir::currentPath(),
                   tr("���� CSV (����������� ����� � �������) (*.csv)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << tr("������ �������� ����� %1").arg(fileName).toStdString()
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
            QString lname = fields.takeFirst().trimmed();
            QString fname = fields.takeFirst().trimmed();
            QString mname = fields.takeFirst().trimmed();
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
    msg.setText(tr("������ ��������."));
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

    employeeVisitLstModel->setQuery(getEmplVisitSql());
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("�����"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    aboutAction = new QAction(tr("� ���������"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    showVisitsAction = new QAction(tr("������"), this);
    connect(showVisitsAction, SIGNAL(triggered()), this, SLOT(showVisits()));

    showEmployeesAction = new QAction(tr("����������"), this);
    connect(showEmployeesAction, SIGNAL(triggered()), this, SLOT(showEmployees()));

    reportSimpleAction = new QAction(tr("������"), this);
    connect(reportSimpleAction, SIGNAL(triggered()), this, SLOT(showReportSimple()));

    reportForAccountingAction = new QAction(tr("��� �����������"), this);
    connect(reportForAccountingAction, SIGNAL(triggered()),
            this, SLOT(showReportAccounting()));

    importEmplCSVAction = new QAction(tr("������ ����������� �� CSV"), this);
    connect(importEmplCSVAction, SIGNAL(triggered()), this, SLOT(importEmplCSV()));

    addEmployeeAction = new QAction(tr("�������� ����������"), this);
    addEmployeeAction->setIcon(QIcon(":/images/add_file.png"));
    addEmployeeAction->setToolTip(tr("�������� ����������"));
    connect(addEmployeeAction, SIGNAL(triggered()), this, SLOT(addEmployee()));

    editEmployeeAction = new QAction(tr("������������� ����������"), this);
    editEmployeeAction->setIcon(QIcon(":/images/edit.png"));
    editEmployeeAction->setToolTip(tr("������������� ����������"));
    connect(editEmployeeAction, SIGNAL(triggered()), this, SLOT(editEmployee()));

    delEmployeeAction = new QAction(tr("������� ����������"), this);
    delEmployeeAction->setIcon(QIcon(":/images/delete.png"));
    delEmployeeAction->setToolTip(tr("������� ����������"));
    connect(delEmployeeAction, SIGNAL(triggered()), this, SLOT(delEmployee()));

    addEmployeeVisitAction = new QAction(tr("�������� ����� ����������"), this);
    addEmployeeVisitAction->setIcon(QIcon(":/images/add_file.png"));
    addEmployeeVisitAction->setToolTip(tr("�������� ����� ����������"));
    connect(addEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(addEmployeeVisit()));

    editEmployeeVisitAction = new QAction(tr("������������� ����� ����������"), this);
    editEmployeeVisitAction->setIcon(QIcon(":/images/edit.png"));
    editEmployeeVisitAction->setToolTip(tr("������������� ����� ����������"));
    connect(editEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(editEmployeeVisit()));

    delEmployeeVisitAction = new QAction(tr("������� ����� ����������"), this);
    delEmployeeVisitAction->setIcon(QIcon(":/images/delete.png"));
    delEmployeeVisitAction->setToolTip(tr("������� ����� ����������"));
    connect(delEmployeeVisitAction, SIGNAL(triggered()), this, SLOT(delEmployeeVisit()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("����"));
    fileMenu->addAction(exitAction);

    listsMenu = menuBar()->addMenu(tr("������"));
    listsMenu->addAction(showVisitsAction);
    listsMenu->addAction(showEmployeesAction);

    reportsMenu = menuBar()->addMenu(tr("������"));
    reportsMenu->addAction(reportSimpleAction);
    reportsMenu->addAction(reportForAccountingAction);

    systemMenu = menuBar()->addMenu(tr("���������"));
    systemMenu->addAction(importEmplCSVAction);

    helpMenu = menuBar()->addMenu(tr("������"));
    helpMenu->addAction(aboutAction);

}

void MainWindow::createToolBars()
{
    employeeToolBar = addToolBar(tr("������ ����������"));
    employeeToolBar->addAction(addEmployeeAction);
    employeeToolBar->addAction(editEmployeeAction);
    employeeToolBar->addAction(delEmployeeAction);
    employeeToolBar->hide();

    employeeVisitToolBar = addToolBar(tr("������ ������ ����������"));
    employeeVisitToolBar->addAction(addEmployeeVisitAction);
    // employeeVisitToolBar->addAction(editEmployeeVisitAction);
    employeeVisitToolBar->addAction(delEmployeeVisitAction);

}

