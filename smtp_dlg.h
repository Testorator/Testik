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

private:
    Ui::smtp_dlg *ui;
};

#endif // SMTP_DLG_H
