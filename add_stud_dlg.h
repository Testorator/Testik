#ifndef ADD_STUD_DLG_H
#define ADD_STUD_DLG_H

#include <QDialog>

struct stud_data{
    int grp_id;
    QString grp_code;
    int stud_id;
    QString stud_surename;
    QString stud_name;
    QString stud_patronymic;
};

namespace Ui {
class add_stud_dlg;
}

class add_stud_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit add_stud_dlg(stud_data *data,QWidget *parent = 0);
    ~add_stud_dlg();

private:
    Ui::add_stud_dlg *ui;
};

#endif // ADD_STUD_DLG_H
