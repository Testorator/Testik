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

private:
    Ui::add_stud_dlg *ui;
};

#endif // ADD_STUD_DLG_H
