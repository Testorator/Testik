#ifndef ADMIN_FORM_ADD_H
#define ADMIN_FORM_ADD_H

#include <QDialog>

namespace Ui {
class admin_form_add;
}

class admin_form_add : public QDialog
{
    Q_OBJECT

public:
    explicit admin_form_add(QWidget *parent = 0);
    ~admin_form_add();

private:
    Ui::admin_form_add *ui;
};

#endif // ADMIN_FORM_ADD_H
