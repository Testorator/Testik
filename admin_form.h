#ifndef ADMIN_FORM_H
#define ADMIN_FORM_H

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

private:
    Ui::admin_form *ui;
};

#endif // ADMIN_FORM_H
