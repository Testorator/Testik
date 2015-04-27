#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"

#include <QToolBar>

question_mod_dialog::question_mod_dialog(QList<st_answer> *answers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::question_mod_dialog)
{
    ui->setupUi(this);
//     = answers;

    QToolBar* ptb = new QToolBar("answers toolbar");
    ptb->setIconSize(QSize(24, 24));
    addAns = new QAction(QIcon(":/resourse/add"),tr("Add"),ptb);
    connect(this->addAns,SIGNAL(triggered()),this,SLOT(on_addAns_triggered()));
    saveAns = new QAction(QIcon(":/resourse/save"),tr("Save"),ptb);
    delAns = new QAction(QIcon(":/resourse/erase"),tr("Del"),ptb);
    ptb->addAction(addAns);
    ptb->addSeparator();
    ptb->addAction(saveAns);
    ptb->addSeparator();
    ptb->addAction(delAns);
    ui->gridLayout_Answers_tb->addWidget(ptb,0,0,0,2,Qt::AlignTop);

    ui->comboBox_Themes->clear();
    ui->tableWidget_Andwers->setColumnCount(3);
    loadAnswers(answers_from_db);

}
//
question_mod_dialog::~question_mod_dialog()
{
    delete ui;
}
//
void question_mod_dialog::addThemeToList(QString theme_name, QVariant theme_id)
{
    ui->comboBox_Themes->addItem(theme_name,theme_id);
}
//
void question_mod_dialog::setCurrentTheme(QVariant theme_id)
{
    ui->comboBox_Themes->setCurrentIndex(ui->comboBox_Themes->findData(theme_id,Qt::UserRole,Qt::MatchExactly));
}
//
QString question_mod_dialog::getQuestion()
{
    return ui->textEdit_Question->toPlainText().trimmed();
}
//
QVariant question_mod_dialog::getQuestionTheme()
{
    return ui->comboBox_Themes->currentData();
}
//
void question_mod_dialog::setQuestionText(QString text)
{
    ui->textEdit_Question->clear();
    ui->textEdit_Question->setText(text);
}
//
void question_mod_dialog::loadAnswers(QList<st_answer> *answers)
{
    ui->tableWidget_Andwers->clear();
    ui->tableWidget_Andwers->setRowCount(answers->count());
    for(int a=0;a<answers->count();a++){
//        ui->tableWidget_Andwers->
    }

}
//
void question_mod_dialog::on_addAns_triggered()
{
    bool ok = true;
    if(ui->textEdit_Answer->toPlainText().trimmed().length() == 0) ok = false;

}
