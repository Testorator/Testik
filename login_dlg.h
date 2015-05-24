#ifndef LOGIN_DLG_H
#define LOGIN_DLG_H

#include <QDialog>

namespace Ui {
class login_dlg;
}

class login_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit login_dlg(QWidget *parent = 0);
    ~login_dlg();

private:
    Ui::login_dlg *ui;
};

#endif // LOGIN_DLG_H
