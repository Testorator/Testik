#ifndef STUD_DLG_H
#define STUD_DLG_H

#include <QDialog>

namespace Ui {
class stud_dlg;
}

class stud_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit stud_dlg(QWidget *parent = 0);
    ~stud_dlg();
    void comboBox_groups_clear();
    void comboBox_groups_addItem(QString item, QVariant itemID);
    void comboBox_groups_set_curItem(QString item);
    void lineEdit_Name_setText(QString value);
    void lineEdit_Surname_setText(QString value);
    void lineEdit_Patronymic_setText(QString value);
    QVariant get_group_id();
    QString get_group_code();
    QString get_lineEdit_Name();
    QString get_lineEdit_Surname();
    QString get_lineEdit_Patronymic();

private:
    Ui::stud_dlg *ui;

};

#endif // ADD_STUD_DLG_H
