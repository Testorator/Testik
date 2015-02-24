#ifndef CHANGE_ADMIN_PW_DIALOG_H
#define CHANGE_ADMIN_PW_DIALOG_H

#include <QDialog>

namespace Ui {
class change_admin_pw_dialog;
}

class change_admin_pw_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit change_admin_pw_dialog(QWidget *parent = 0);
    ~change_admin_pw_dialog();

private:
    Ui::change_admin_pw_dialog *ui;
};

#endif // CHANGE_ADMIN_PW_DIALOG_H
