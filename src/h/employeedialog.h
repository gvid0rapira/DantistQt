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
     * \brief Инициализация режима редактирования работы формы.
     *
     * Форма работает в двух режимах: редактирование или добавление. Этот метод устанавливает режим
     * формы в соответствии с переданным параметром row:
     *  -1 - добавление,
     * целое больше -1 - редактирование.
     */
    void rowToEdit(int row);
private slots:
    void acceptInput();
    void rejectInput();

private:
    //--*- режимы
    FormMode mode;

    //--*- Список элементов интерфейса
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

    //--*- Объекты для работы с данными
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EMPLOYEEDIALOG_H
