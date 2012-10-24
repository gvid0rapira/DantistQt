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
#include <QAxObject>
#include <QDateEdit>

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

    static QString getEmplVisitSql() {


        return QString("SELECT v.id, e.lname, e.fname, e.mname, e.tab_num, v.visit_date ")
                + QString("FROM employee e, visit v ")
                + QString("WHERE v.employee_id = e.id");
    }
    
private slots:
    void exit();
    void about();
    void showVisits();
    void showEmployees();

    void showReportSimple();
    void makeReportSimple();
    void showReportAccounting();
    void makeReportAccounting();

    void addEmployee();
    void editEmployee();
    void delEmployee();

    void addEmployeeVisit();
    void editEmployeeVisit();
    void delEmployeeVisit();

    void importEmplCSV();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    void drawCellBorders(QAxObject &sheet, int row, int colCount); // Рисование гнаниц ячеек отчета

private:
    Ui::MainWindow *ui;

    // Модель и вид для списка визитов
    QSqlQueryModel *employeeVisitLstModel;
    QTableView  *employeeVisitLstView;

    // Модель и вид для списка сотрудников
    QSqlTableModel *employeeLstModel;
    QTableView  *employeeLstView;

    QWidget *reportSimplePanel;        // Панель простого отчета
    QLabel *repSimpleTitleLbl;
    QLabel *repSimpleFromLbl;
    QDateEdit *repSimpleFromDEdit;
    QLabel *repSimpleToLbl;
    QDateEdit *repSimpleToDEdit;
    QPushButton   *repSimpleMakeBtn;

    QWidget *repAccPanel; // Панель отчета для бухгалтерии
    QLabel *repAccTitleLbl;
    QLabel *repAccFromLbl;
    QDateEdit *repAccFromDEdit;
    QLabel *repAccToLbl;
    QDateEdit *repAccToDEdit;
    QPushButton *repAccMakeBtn;

    QStackedWidget *stackedWidget;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *listsMenu;
    QMenu *reportsMenu;
    QMenu *systemMenu;

    QToolBar *mainToolBar;
    QToolBar *employeeToolBar;
    QToolBar *employeeVisitToolBar;

    QAction *exitAction;
    QAction *showVisitsAction;
    QAction *showEmployeesAction;
    QAction *reportSimpleAction;
    QAction *reportForAccountingAction;
    QAction *aboutAction;
    QAction *importEmplCSVAction;

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
