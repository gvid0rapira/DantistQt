#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/h/db/dbstructure.h"

#include <QtSql>
#include <QIcon>
#include <QGridLayout>
#include <QDateTimeEdit>

#include <QModelIndexList>
#include <QModelIndex>

#include <QFile>

#include <QAxObject>

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

    // ������ ������ �������
    employeeVisitLstModel = new QSqlRelationalTableModel(this);
    employeeVisitLstModel->setTable("visit");
    employeeVisitLstModel->setRelation(Visit_Employee_Id,
                                       QSqlRelation("employee", "id", "lname"));
    employeeVisitLstModel->select();

    employeeVisitLstView = new QTableView;
    employeeVisitLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeVisitLstView->setModel(employeeVisitLstModel);

    // ������ �������� ������
    reportSimplePanel   = new QWidget();
    repSimpleTitleLbl   = new QLabel(tr("������� �����"), this);
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

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(employeeLstView);
    stackedWidget->addWidget(employeeVisitLstView);
    stackedWidget->addWidget(reportSimplePanel);

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


    // �������� ����� ������ �� �������
    QString tmplFileName("d:\\Projects\\Work\\DantistQt\\templates\\register.xlsx");
    QString regFileName(QString("d:\\Projects\\Work\\DantistQt\\reports\\register_%1.xlsx")
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
                ->setProperty("Value", QVariant(visitTime));

        // ��������� ������
        drawRowRegister(*sheet, startRow + count);
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

void MainWindow::drawRowRegister(QAxObject &sheet, int row)
{
    sheet.querySubObject("Cells(Int, Int)", row, 1 )
            ->querySubObject("Borders(Int)", 7)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 1 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 1 )
            ->querySubObject("Borders(Int)", 10)
            ->setProperty("LineStyle", 1);

    sheet.querySubObject("Cells(Int, Int)", row, 2 )
            ->querySubObject("Borders(Int)", 7)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 2 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 2 )
            ->querySubObject("Borders(Int)", 10)
            ->setProperty("LineStyle", 1);

    sheet.querySubObject("Cells(Int, Int)", row, 3 )
            ->querySubObject("Borders(Int)", 7)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 3 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 3 )
            ->querySubObject("Borders(Int)", 10)
            ->setProperty("LineStyle", 1);

    sheet.querySubObject("Cells(Int, Int)", row, 4 )
            ->querySubObject("Borders(Int)", 7)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 4 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 4 )
            ->querySubObject("Borders(Int)", 10)
            ->setProperty("LineStyle", 1);

    sheet.querySubObject("Cells(Int, Int)", row, 5 )
            ->querySubObject("Borders(Int)", 7)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 5 )
            ->querySubObject("Borders(Int)", 9)
            ->setProperty("LineStyle", 1);
    sheet.querySubObject("Cells(Int, Int)", row, 5 )
            ->querySubObject("Borders(Int)", 10)
            ->setProperty("LineStyle", 1);
}


void MainWindow::showReportAccounting(){}

void MainWindow::makeReportAccounting(){}

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
    employeeVisitLstModel->removeRow(row);
    employeeVisitLstModel->submitAll();
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

    reportSimpleAction = new QAction(tr("�������"), this);
    connect(reportSimpleAction, SIGNAL(triggered()), this, SLOT(showReportSimple()));

    reportForAccountingAction = new QAction(tr("��� �����������"), this);

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
    employeeVisitToolBar->addAction(editEmployeeVisitAction);
    employeeVisitToolBar->addAction(delEmployeeVisitAction);

}

