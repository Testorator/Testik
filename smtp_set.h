#ifndef SMTP_SET_H
#define SMTP_SET_H

#include <QDialog>
#include <pub_struct.h>

namespace Ui {
class smtp_set;
}

class smtp_set : public QDialog
{
    Q_OBJECT

public:
    explicit smtp_set(QList<st_recipient> *addreses, QWidget *parent = 0);
    ~smtp_set();
    QString getServer();
    void setServer(QString value);
    int getPort();
    void setPort(int value);
    bool getUseSSL();
    void setUseSSL(bool value);
    QString getMailFrom();
    void setMailFrom(QString value);
    QString getLogin();
    void setLogin(QString value);
    QString getPassword();
    void setPassword(QString value);

private slots:
    void on_toolButton_SendTestMsg_clicked();

    void on_lineEdit_mailFrom_editingFinished();

private:
    Ui::smtp_set *ui;
    QList<st_recipient> *addr_list;
};

#endif // SMTP_SET_H
