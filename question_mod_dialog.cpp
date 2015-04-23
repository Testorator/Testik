#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"

#include <QToolBar>

question_mod_dialog::question_mod_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::question_mod_dialog)
{
    ui->setupUi(this);
    QToolBar* ptb = new QToolBar("answers toolbar");
    ptb->addAction(QIcon(":/resourse/add"),tr("Add"));
    ptb->setIconSize(QSize(24, 24));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/resourse/save"),tr("Save"));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/resourse/del"),tr("Del"));
    ui->gridLayout_Answers_tb->addWidget(ptb,0,0,0,2,Qt::AlignTop);
    ui->comboBox_Themes->clear();

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
