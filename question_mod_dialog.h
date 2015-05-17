#ifndef QUESTION_MOD_DIALOG_H
#define QUESTION_MOD_DIALOG_H

#include "pub_struct.h"
#include <QDialog>
#include <QTableWidgetItem>
#include <QCheckBox>

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
    QVariant getIndexBox();
    QString getComment();
    bool uniqueAns();
    QString getanswer();
private:
    int lastIndexOfAnswersType;
    Ui::question_mod_dialog *ui;
    QList<st_answer> *answers_from_db;
    QList<st_answer> answers;
    QList< QCheckBox* > answersChecks;
    QAction *addAns, *saveAns, *delAns;
    void loadAnswers(QList<st_answer> *answers);
    bool getAnswerCheckAvailablity(bool oneCheckException=false);
    int correctAnswersCount();
    void removeAnswer(int row, bool quiet=false);

private slots:
    void clear_AnswerList();
    void on_addAns_triggered();
    void on_editAns_triggered();
    void on_delAns_triggered();
    void on_comboBox_Type_currentIndexChanged(int index);
};

#endif // QUESTION_MOD_DIALOG_H
