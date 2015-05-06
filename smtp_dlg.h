#ifndef SMTP_DLG_H
#define SMTP_DLG_H

#include <QDialog>

namespace Ui {
class smtp_dlg;
}

class smtp_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit smtp_dlg(QWidget *parent = 0);
    ~smtp_dlg();
    void setPort(QVariant value);
    void setServer(QVariant value);
    QVariant getPort();
    QVariant getServer();

private:
    Ui::smtp_dlg *ui;
};

#endif // SMTP_DLG_H
