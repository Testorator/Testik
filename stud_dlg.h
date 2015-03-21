#ifndef ADD_STUD_DLG_H
#define ADD_STUD_DLG_H

#include <QDialog>

namespace Ui {
class add_stud_dlg;
}

class add_stud_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit add_stud_dlg(QWidget *parent = 0);
    ~add_stud_dlg();
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
    Ui::add_stud_dlg *ui;

};

#endif // ADD_STUD_DLG_H
