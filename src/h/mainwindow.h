#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QStackedWidget>

#include <src/h/employeedialog.h>
#include <src/h/employeevisitdialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void exit();
    void about();
    void showVisits();
    void showEmployees();

    void addEmployee();
    void editEmployee();
    void delEmployee();

    void addEmployeeVisit();
    void editEmployeeVisit();
    void delEmployeeVisit();

private:
    void createActions();
    void createMenus();
    void createToolBars();

private:
    Ui::MainWindow *ui;

    // Модель и вид для списка визитов
    QSqlRelationalTableModel *employeeVisitLstModel;
    QTableView  *employeeVisitLstView;

    // Модель и вид для списка сотрудников
    QSqlTableModel *employeeLstModel;
    QTableView  *employeeLstView;

    QStackedWidget *stackedWidget;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *listsMenu;
    QMenu *reportsMenu;

    QToolBar *mainToolBar;
    QToolBar *employeeToolBar;
    QToolBar *employeeVisitToolBar;

    QAction *exitAction;
    QAction *showVisitsAction;
    QAction *showEmployeesAction;
    QAction *aboutAction;

    QAction *addEmployeeAction;
    QAction *editEmployeeAction;
    QAction *delEmployeeAction;

    QAction *addEmployeeVisitAction;
    QAction *editEmployeeVisitAction;
    QAction *delEmployeeVisitAction;

    EmployeeDialog *employeeDlg;
    EmployeeVisitDialog *employeeVisitDlg;
};

#endif // MAINWINDOW_H
