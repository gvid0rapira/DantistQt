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
    // �������� �������, �.�. ������-��, ����� ����������, ��������� ��
    // ������� �� ����� 0.
    this->diagnDlg = 0;
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

    // ������ ������ ���������
    diagnLstModel = new QSqlTableModel(this);
    diagnLstModel->setTable("diagnosis");
    diagnLstModel->setHeaderData(Diagnosis_name, Qt::Horizontal, tr("��������"));
    diagnLstModel->select();

    diagnLstView = new QTableView;
    diagnLstView->setSelectionBehavior(QAbstractItemView::SelectRows);
    diagnLstView->setModel(diagnLstModel);
    diagnLstView->setColumnHidden(Diagnosis_Id, true);
    diagnLstView->resizeColumnsToContents();

    // ������ ������ �������
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

    visitFilterApplyBtn = new QPushButton(tr("��������") ,this);
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
                                         tr("�������"));
    employeeVisitLstModel->setHeaderData(2, Qt::Horizontal,
                                         tr("���"));
    employeeVisitLstModel->setHeaderData(3, Qt::Horizontal,
                                         tr("��������"));
    employeeVisitLstModel->setHeaderData(4, Qt::Horizontal,
                                         tr("���. �"));
    employeeVisitLstModel->setHeaderData(5, Qt::Horizontal,
                                         tr("���� ���������"));
    employeeVisitLstModel->setHeaderData(6, Qt::Horizontal,
                                         tr("���"));
    employeeVisitLstModel->setHeaderData(7, Qt::Horizontal,
                                         tr("�������"));

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


    // ������ �������� ������
    reportSimplePanel   = new QWidget();
    repSimpleTitleLbl   = new QLabel(tr("������"), this);
    repSimpleFromLbl    = new QLabel(tr("�"), this);
    repSimpleFromDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                             QDate::currentDate().month(),
                                             1), this);
    repSimpleToLbl      = new QLabel(tr("��"), this);
    repSimpleToDEdit   = new QDateEdit(QDate(QDate::currentDate().year(),
                                             QDate::currentDate().month(),
                                             QDate::currentDate().daysInMonth()), this);
    repSimpleMakeBtn    = new QPushButton(tr("������������"));
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

    // ������ ������ ��� �����������
    repAccPanel = new QWidget();
    repAccTitleLbl = new QLabel(tr("����� ��� �����������"), this);
    repAccFromLbl = new QLabel(tr("�"), this);
    repAccFromDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                          QDate::currentDate().month(),
                                          1), this);
    repAccToLbl = new QLabel(tr("��"), this);
    repAccToDEdit = new QDateEdit(QDate(QDate::currentDate().year(),
                                        QDate::currentDate().month(),
                                        QDate::currentDate().daysInMonth()), this);
    repAccMakeBtn =  new QPushButton(tr("������������"));
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
    stackedWidget->addWidget(diagnLstView);

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

void MainWindow::showDiagnosises()
{
    employeeVisitToolBar->hide();
    employeeToolBar->hide();
    diagnToolBar->show();
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showEmployees()
{
    employeeVisitToolBar->hide();
    employeeToolBar->show();
    diagnToolBar->hide();
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showVisits()
{
    employeeToolBar->hide();
    employeeVisitToolBar->show();
    diagnToolBar->hide();
    stackedWidget->setCurrentIndex(1);
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

    QString sql = QString("SELECT e.lname, e.fname, e.mname, v.visit_date, d.name ")
            + QString("FROM employee e, visit v, diagnosis d ")
            + QString("WHERE (date(v.visit_date) BETWEEN date('%1') AND date('%2'))")
                .arg(from.toString("yyyy-MM-dd"))
                .arg(to.toString("yyyy-MM-dd"))
            + QString(" AND  v.employee_id = e.id")
            + QString(" AND  v.diagnosis_id = d.id");
    qDebug() << sql;
    QSqlQuery query(sql);
    if(query.isActive()) {
        query.finish();
    }
    query.exec();

    // �������� ����� �� ���� �� �������
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
        QDate visiDate = query.value(3).toDate();
        QString diagnosis = query.value(4).toString();
        qDebug() << lname << fname << mname;
        count++;
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 1 )
                ->setProperty("Value", QVariant(count));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 2 )
                ->setProperty("Value", QVariant(lname + " " + fname + " " + mname));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 4 )
                ->setProperty("Value", QVariant(visiDate.toString("dd.MM.yyyy")));
        sheet->querySubObject("Cells(Int, Int)", startRow + count, 5 )
                ->setProperty("Value", QVariant(diagnosis));

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
    QDate from = repAccFromDEdit->date();
    QDate to   = repAccToDEdit->date();

    // TODO: ������� ��������� 750 � ���������������� ����
    QString sql = QString("SELECT DISTINCT e.lname, e.fname, e.mname, e.tab_num, e.work_place, ")
            + QString("v.visit_date, sum(v.uet), sum(v.uet)*750 ")
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

    // �������� ����� ������ �� �������

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
    float allUet = 0;
    float totalSpent =0;
    QLocale locale(QLocale::Russian, QLocale::RussianFederation);// ��� �������������� �����
    while( query.next() ) {
        QString lname = query.value(0).toString();
        QString fname = query.value(1).toString();
        QString mname = query.value(2).toString();
        int tab_num   = query.value(3).toInt();
        QString work_place = query.value(4).toString();
        float uet_sum = query.value(6).toFloat();
        allUet += uet_sum;
        float sum_spent = query.value(7).toFloat();
        totalSpent += sum_spent;

        qDebug() << lname << " " << fname << " " << mname << ", "
                 << tab_num << ", " << work_place << ", " << uet_sum << ", " << sum_spent;

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
                ->setProperty("Value", QVariant(locale.toString(uet_sum)));

        sheet->querySubObject("Cells(Int, Int)", startRow + rowCount, 6 )
                ->setProperty("Value", QVariant(locale.toString(sum_spent)));
        drawCellBorders(*sheet, startRow + rowCount, 7);
    }

    // ������� ������
    drawCellBorders(*sheet, startRow + rowCount + 1, 7);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("HorizontalAlignment", -4152);
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 2 )
            ->setProperty("Value", QVariant(tr("�����:")));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 5 )
            ->setProperty("Value", locale.toString(allUet));
    sheet->querySubObject("Cells(Int, Int)", startRow + rowCount + 1, 6 )
            ->setProperty("Value", locale.toString(totalSpent));

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
    msg.setText(tr("������ ��������."));
    msg.exec();
}

void MainWindow::addDiagnosis(){
    if(!diagnDlg){
        diagnDlg = new DiagnosisDialog(diagnLstModel, this);
    }

    diagnDlg->initEditMode(-1);
    diagnDlg->show();
    diagnDlg->raise();
    diagnDlg->activateWindow();
}

void MainWindow::delDiagnosis()
{
    QModelIndexList list = diagnLstView->selectionModel()->selection().indexes();
    if(list.size() == 0){
        QMessageBox::warning(this, tr("������ ��������"),
                             tr("�� ������� ������"));
        return;
    }
    QModelIndex index = list.first();
    int row = index.row();
    diagnLstModel->removeRow(row);
    diagnLstModel->submitAll();
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
    if(list.size() == 0){
        QMessageBox::warning(this, tr("������ ��������������"),
                             tr("�� ������� ������"));
        return;
    }
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
    if(list.size() == 0){
        QMessageBox::warning(this, tr("������ ��������"),
                             tr("�� ������� ������"));
        return;
    }
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
    exitAction = new QAction(tr("�����"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    aboutAction = new QAction(tr("� ���������"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    showVisitsAction = new QAction(tr("������"), this);
    connect(showVisitsAction, SIGNAL(triggered()), this, SLOT(showVisits()));

    showDiagnAction = new QAction(tr("��������"), this);
    connect(showDiagnAction, SIGNAL(triggered()), this, SLOT(showDiagnosises()));

    addDiagnAction = new QAction(tr("�������� �������"), this);
    addDiagnAction->setIcon(QIcon(":/images/add_file.png"));
    addDiagnAction->setToolTip(tr("�������� �������"));
    addDiagnAction->setShortcut(QKeySequence(Qt::Key_Insert));
    connect(addDiagnAction, SIGNAL(triggered()), this, SLOT(addDiagnosis()));

    delDiagnAction = new QAction(tr("������� �������"), this);
    delDiagnAction->setIcon(QIcon(":/images/delete.png"));
    delDiagnAction->setToolTip(tr("������� �������"));
    connect(delDiagnAction, SIGNAL(triggered()), this, SLOT(delDiagnosis()));

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
    addEmployeeAction->setShortcut(QKeySequence(Qt::Key_Insert));
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
    addEmployeeVisitAction->setShortcut(QKeySequence(Qt::Key_Insert));
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
    listsMenu->addAction(showDiagnAction);
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

/*! \brief ����� ������� ��� �������.
 *
 *   ����� ���������������� �����������/������� ��������.
 *
 */
void MainWindow::createToolBars()
{
    diagnToolBar = addToolBar(tr("������ ��������"));
    diagnToolBar->addAction(addDiagnAction);
    diagnToolBar->addAction(delDiagnAction);
    diagnToolBar->hide();

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

