#ifndef EMPLOYEEDIALOG_H
#define EMPLOYEEDIALOG_H

#include <src/h/gui_common.h>

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QSqlTableModel>
#include <QDataWidgetMapper>


class EmployeeDialog : public QDialog
{
    Q_OBJECT


public:
    explicit EmployeeDialog(int row, QSqlTableModel *m, QWidget *parent = 0);
    ~EmployeeDialog();

    /*!
     * \brief ������������� ������ �������������� ������ �����.
     *
     * ����� �������� � ���� �������: �������������� ��� ����������. ���� ����� ������������� �����
     * ����� � ������������ � ���������� ���������� row:
     *  -1 - ����������,
     * ����� ������ -1 - ��������������.
     */
    void rowToEdit(int row);
private slots:
    void acceptInput();
    void rejectInput();

private:
    //--*- ������
    FormMode mode;

    //--*- ������ ��������� ����������
    QLabel *lnameLabel;
    QLabel *fnameLabel;
    QLabel *mnameLabel;
    QLabel *tabNumLabel;
    QLabel *depLabel;

    QLineEdit *lnameEdit;
    QLineEdit *fnameEdit;
    QLineEdit *mnameEdit;
    QLineEdit *tabNumEdit;
    QLineEdit *depEdit;

    QPushButton *acceptButton;
    QPushButton *rejectButton;

    //--*- ������� ��� ������ � �������
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EMPLOYEEDIALOG_H
