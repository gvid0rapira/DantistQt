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

private:
    void createActions();
    void createMenus();
    void createToolBars();

    void drawCellBorders(QAxObject &sheet, int row, int colCount); // ��������� ������ ����� ������

private:
    Ui::MainWindow *ui;

    // ������ � ��� ��� ������ �������
    QSqlRelationalTableModel *employeeVisitLstModel;
    QTableView  *employeeVisitLstView;

    // ������ � ��� ��� ������ �����������
    QSqlTableModel *employeeLstModel;
    QTableView  *employeeLstView;

    QWidget *reportSimplePanel;        // ������ �������� ������
    QLabel *repSimpleTitleLbl;
    QLabel *repSimpleFromLbl;
    QDateTimeEdit *repSimpleFromDTEdit;
    QLabel *repSimpleToLbl;
    QDateTimeEdit *repSimpleToDTEdit;
    QPushButton   *repSimpleMakeBtn;

    QWidget *repAccPanel; // ������ ������ ��� �����������
    QLabel *repAccTitleLbl;
    QLabel *repAccFromLbl;
    QDateTimeEdit *repAccFromDTEdit;
    QLabel *repAccToLbl;
    QDateTimeEdit *repAccToDTEdit;
    QPushButton *repAccMakeBtn;

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
    QAction *reportSimpleAction;
    QAction *reportForAccountingAction;
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
