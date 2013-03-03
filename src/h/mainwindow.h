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

#include <src/h/diagnosisdialog.h>
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

    static QSqlQuery getEmplVisitQuery(QDate from, QDate to) {

        return QString("SELECT v.id, e.lname, e.fname, e.mname, e.tab_num, ")
                + QString("v.visit_date, v.uet, d.name ")
                + QString("FROM employee e, visit v, diagnosis d ")
                + QString("WHERE v.employee_id = e.id AND ")
                + QString("v.diagnosis_id = d.id AND ")
                + QString("(v.visit_date BETWEEN \"%1\" AND \"%2\")")
                .arg(from.toString("yyyy-MM-dd"))
                .arg(to.toString("yyyy-MM-dd"));

    }
public slots:
    void updateVisitsLst();
    
private slots:
    void exit();
    void about();
    void showVisits();
    void showEmployees();
    void showDiagnosises();

    void showReportSimple();
    void makeReportSimple();
    void showReportAccounting();
    void makeReportAccounting();

    void addEmployee();
    void editEmployee();
    void delEmployee();

    void addDiagnosis();
    void delDiagnosis();

    void addEmployeeVisit();
    void editEmployeeVisit();
    void delEmployeeVisit();

    void importEmplCSV();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    // Рисование гнаниц ячеек отчета
    void drawCellBorders(QAxObject &sheet, int row, int colCount);

private:
    Ui::MainWindow *ui;

    // Модель и вид для списка визитов
    QSqlQuery *employeeVisitLstQuery;
    QSqlQueryModel *employeeVisitLstModel;
    QTableView  *employeeVisitLstView;
    QDateEdit *visitFilterFrom;
    QDateEdit *visitFilterTo;
    QPushButton *visitFilterApplyBtn;

    // Модель и вид для списка сотрудников
    QSqlTableModel *employeeLstModel;
    QTableView  *employeeLstView;

    // Модель и вид для списка диагнозов
    QSqlTableModel *diagnLstModel;
    QTableView  *diagnLstView;

    QWidget *reportSimplePanel; // Панель простого отчета
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
    QToolBar *diagnToolBar;
    QToolBar *employeeVisitToolBar;

    QAction *exitAction;
    QAction *showVisitsAction;
    QAction *showEmployeesAction;
    QAction *showDiagnAction;
    QAction *reportSimpleAction;
    QAction *reportForAccountingAction;
    QAction *aboutAction;
    QAction *importEmplCSVAction;

    QAction *addEmployeeAction;
    QAction *editEmployeeAction;
    QAction *delEmployeeAction;

    QAction *addDiagnAction;
    QAction *delDiagnAction;

    QAction *addEmployeeVisitAction;
    QAction *editEmployeeVisitAction;
    QAction *delEmployeeVisitAction;

    DiagnosisDialog     *diagnDlg;
    EmployeeDialog      *employeeDlg;
    EmployeeVisitDialog *employeeVisitDlg;
};

#endif // MAINWINDOW_H
