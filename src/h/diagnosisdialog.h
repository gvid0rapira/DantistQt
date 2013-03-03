#ifndef DIAGNOSISDIALOG_H
#define DIAGNOSISDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

#include <src/h/gui_common.h>

namespace Ui {
class DiagnosisDialog;
}

class DiagnosisDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DiagnosisDialog(QSqlTableModel *m, QWidget *parent = 0);
    ~DiagnosisDialog();

    /*!
     * \brief ������������� ������ �������������� ������ �����.
     *
     * ����� �������� � ���� �������: �������������� ��� ����������. ���� ����� ������������� �����
     * ����� � ������������ � ���������� ���������� row:
     *  -1 - ����������,
     * ����� ������ -1 - ��������������.
     */
    void initEditMode(int row);
    
private slots:
    void on_cancelBtn_clicked();

    void on_OKBtn_clicked();

private:
    Ui::DiagnosisDialog *ui;

    //--*- ������ ������
    FormMode mode;

    //--*- ������� ��� ������ � �������
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // DIAGNOSISDIALOG_H
