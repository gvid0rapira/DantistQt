#include "src/h/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/h/db/dbstructure.h"

#include <QtSql>
#include <QIcon>

#include <QModelIndexList>
#include <QModelIndex>

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

    employeeLstModel = new QSqlTableModel(this);
    employeeLstModel->setTable("employee");
    employeeLstModel->select();

    employeeLstView = new QTableView;
    employeeLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeLstView->setModel(employeeLstModel);

    employeeVisitLstModel = new QSqlRelationalTableModel(this);
    employeeVisitLstModel->setTable("visit");
    employeeVisitLstModel->setRelation(Visit_Employee_Id,
                                       QSqlRelation("employee", "id", "lname"));
    employeeVisitLstModel->select();

    employeeVisitLstView = new QTableView;
    employeeVisitLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeVisitLstView->setModel(employeeVisitLstModel);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(employeeLstView);
    stackedWidget->addWidget(employeeVisitLstView);

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
