#ifndef EMAIL_DLG_H
#define EMAIL_DLG_H

#include <QDialog>

namespace Ui {
class email_dlg;
}

class email_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit email_dlg(QWidget *parent = 0);
    ~email_dlg();
    QString getRecipient();
    void setRecipient(QString value);
    QString getAddress();
    void setAddress(QString value);
    QString getId();
    void setId(QString value);


private:
    Ui::email_dlg *ui;
};

#endif // EMAIL_DLG_H
