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
     * \brief »нициализаци€ режима редактировани€ работы формы.
     *
     * ‘орма работает в двух режимах: редактирование или добавление. Ётот метод устанавливает режим
     * формы в соответствии с переданным параметром row:
     *  -1 - добавление,
     * целое больше -1 - редактирование.
     */
    void initEditMode(int row);
    
private slots:
    void on_cancelBtn_clicked();

    void on_OKBtn_clicked();

private:
    Ui::DiagnosisDialog *ui;

    //--*- –ежимы работы
    FormMode mode;

    //--*- ќбъекты дл€ работы с данными
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // DIAGNOSISDIALOG_H
