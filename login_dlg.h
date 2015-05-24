#ifndef LOGIN_DLG_H
#define LOGIN_DLG_H

#include <QDialog>
#include "dbfunc.h"

namespace Ui {
class login_dlg;
}

class login_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit login_dlg(QWidget *parent = 0);
    ~login_dlg();

private slots:
    void on_pushButton_Exit_clicked();
    void on_comboBox_DB_currentIndexChanged(int index);

private:
    Ui::login_dlg *ui;
    sql_cl *sql;
    void getDataBases();
};

#endif // LOGIN_DLG_H
