#ifndef ANSWER_MOD_DLG_H
#define ANSWER_MOD_DLG_H

#include <QDialog>

namespace Ui {
class answer_mod_dlg;
}

class answer_mod_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit answer_mod_dlg(QWidget *parent = 0);
    ~answer_mod_dlg();

private:
    Ui::answer_mod_dlg *ui;
};

#endif // ANSWER_MOD_DLG_H
