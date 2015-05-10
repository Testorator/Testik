#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"
#include "dbfunc.h"
#include <QToolBar>
#include <qmessagebox.h>
question_mod_dialog::question_mod_dialog(QList<st_answer> *answers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::question_mod_dialog)
{
    ui->setupUi(this);
    answers_from_db  = answers;

    QToolBar* ptb = new QToolBar("answers toolbar");
    ptb->setIconSize(QSize(24, 24));
    addAns = new QAction(QIcon(":/resource/add"),tr("Add"),ptb);
    connect(this->addAns,SIGNAL(triggered()),this,SLOT(on_addAns_triggered()));
    saveAns = new QAction(QIcon(":/resource/save"),tr("Save"),ptb);
    connect(this->saveAns,SIGNAL(triggered()),this,SLOT(on_saveAns_triggered()));
    delAns = new QAction(QIcon(":/resource/erase"),tr("Del"),ptb);
    connect(this->delAns,SIGNAL(triggered()),this,SLOT(on_delAns_triggered()));
    ptb->addAction(addAns);
    ptb->addSeparator();
    ptb->addAction(saveAns);
    ptb->addSeparator();
    ptb->addAction(delAns);
    ui->gridLayout_Answers_tb->addWidget(ptb,0,0,0,2,Qt::AlignTop);
    ui->comboBox_Themes->clear();
    ui->tableWidget_Andwers->setColumnCount(2);
    loadAnswers(answers_from_db);
    ui->comboBox_Type->addItem(tr("text entry"), 1);
    ui->comboBox_Type->addItem(tr("one correct answer"), 2);
    ui->comboBox_Type->addItem(tr("many correct answers"), 3);
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
        ui->tableWidget_Andwers->setItem(a,0,new QTableWidgetItem(answers->at(a).ans_text));
        ui->tableWidget_Andwers->setItem(a,1,new QTableWidgetItem(answers->at(a).ans_correct));
    }
}
//
void question_mod_dialog::on_addAns_triggered()
{
    bool ok = true;
    if(ui->textEdit_Answer->toPlainText().trimmed().length() == 0) ok = false;
    else {

                if( ui->tableWidget_Andwers->findItems(getAnswer(), Qt::MatchExactly).isEmpty())
                    {
                        int i = ui->tableWidget_Andwers->rowCount();
                        ui->tableWidget_Andwers->insertRow(i);
                        ui->tableWidget_Andwers->setItem(i,0,new QTableWidgetItem(getAnswer()));
                        if(ui->checkBox_AnsCorrect->isChecked()==true){
                            ui->tableWidget_Andwers->setCellWidget(i,1,new QCheckBox(ui->tableWidget_Andwers));
                                QCheckBox* pCheckB(qobject_cast<QCheckBox*>(ui->tableWidget_Andwers->cellWidget(i, 1)));
                                    pCheckB->setChecked(true);
                                    pCheckB->setDisabled(true);
                                }
                            }

                else
            {
                QMessageBox::critical(new QWidget,QObject::tr("Error"),QObject::tr("This answer ")+
                                                          "\""+getAnswer()+"\""+QObject::tr(" already exists!"));
            }
                    }

     ui->textEdit_Answer->clear();
        }



//
QVariant question_mod_dialog::getIndexBox()
 {
   return ui->comboBox_Type->itemData(ui->comboBox_Type->currentIndex());
 }
//
QString question_mod_dialog::getAnswer()
{
    return ui->textEdit_Answer->toPlainText().trimmed();
}
//
QString question_mod_dialog::getComment()
{
    return ui->textEdit_AnsComment->toPlainText().trimmed();
}
//
void question_mod_dialog::on_saveAns_triggered()
{
QTableWidgetItem *itm = ui->tableWidget_Andwers->currentItem();
ui->textEdit_Answer->setText(itm->text());
}
//
void question_mod_dialog::on_delAns_triggered()
{

    ui->tableWidget_Andwers->removeRow(ui->tableWidget_Andwers->currentRow());
}


void question_mod_dialog::on_tableWidget_Andwers_itemClicked(QTableWidgetItem *item)
{
    ui->textEdit_Answer->setText(item->text());
}
