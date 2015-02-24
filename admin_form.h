#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

#include "change_admin_pw_dialog.h"
#include <QMainWindow>

namespace Ui {
class admin_form;
}

class admin_form : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin_form(QWidget *parent = 0);
    ~admin_form();

private slots:
    void on_actionChange_admin_password_triggered();

private:
    Ui::admin_form *ui;
};

#endif // ADMIN_FORM_H
