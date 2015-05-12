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
    explicit answer_mod_dlg(int ans_type, bool chek_available = 0, QWidget *parent = 0);
    ~answer_mod_dlg();
    QString getAnswerText();
    void setAnswerText(QString value);
    bool getAnswerCorrectFlag();
    void setAnswerCorrectFlag(bool value);

private:
    Ui::answer_mod_dlg *ui;
    int *_ans_type;
};

#endif // ANSWER_MOD_DLG_H
