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
    explicit smtp_set(QList<st_email> *addreses, QWidget *parent = 0);
    ~smtp_set();
    QString getServer();
    void setServer(QString value);
    int getPort();
    void setPort(int value);
    bool getUseSSL();
    void setUseSSL(bool value);
    QString getLogin();
    void setLogin(QString value);
    QString getPassword();
    void setPassword(QString value);

private slots:
    void on_toolButton_SendTestMsg_clicked();

private:
    Ui::smtp_set *ui;
    QList<st_email> *addr_list;
};

#endif // SMTP_SET_H
