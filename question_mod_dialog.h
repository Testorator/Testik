#ifndef QUESTION_MOD_DIALOG_H
#define QUESTION_MOD_DIALOG_H

#include <QDialog>

namespace Ui {
class question_mod_dialog;
}

class question_mod_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit question_mod_dialog(QWidget *parent = 0);
    ~question_mod_dialog();

private:
    Ui::question_mod_dialog *ui;
};

#endif // QUESTION_MOD_DIALOG_H
