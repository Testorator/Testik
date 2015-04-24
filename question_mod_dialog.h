#ifndef QUESTION_MOD_DIALOG_H
#define QUESTION_MOD_DIALOG_H

#include <QDialog>
#include "dbfunc.h"

namespace Ui {
class question_mod_dialog;
}

class question_mod_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit question_mod_dialog(sql_cl *_sql,QWidget *parent = 0);
    ~question_mod_dialog();
    void addThemeToList(QString theme_name, QVariant theme_id);
    void setCurrentTheme(QVariant theme_id);
    void setQuestionText(QString text);
    QString getQuestion();
    QVariant getQuestionTheme();

private:
    Ui::question_mod_dialog *ui;
    sql_cl *sql;
    QAction *addAns, *saveAns, *delAns;
};

#endif // QUESTION_MOD_DIALOG_H
