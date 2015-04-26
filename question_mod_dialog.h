#ifndef QUESTION_MOD_DIALOG_H
#define QUESTION_MOD_DIALOG_H

#include <QDialog>

struct st_answer{
    QString q_id;
    QString ans_id;
    QString ans_text;
    QString ans_comment;
    bool ans_correct;
};

namespace Ui {
class question_mod_dialog;
}

class question_mod_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit question_mod_dialog(QList<st_answer> *answers,QWidget *parent = 0);
    ~question_mod_dialog();
    void addThemeToList(QString theme_name, QVariant theme_id);
    void setCurrentTheme(QVariant theme_id);
    void setQuestionText(QString text);
    QString getQuestion();
    QVariant getQuestionTheme();

private:
    Ui::question_mod_dialog *ui;
    QList<st_answer> *answers_from_db;
    QList<st_answer> answers;
    QAction *addAns, *saveAns, *delAns;
    void loadAnswers(QList<st_answer> *answers);

private slots:
    void on_addAns_triggered();
};

#endif // QUESTION_MOD_DIALOG_H
