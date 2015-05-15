#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"
#include "dbfunc.h"
#include "answer_mod_dlg.h"
#include <QToolBar>
#include <QMessageBox>
#include <QDebug>

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
    saveAns = new QAction(QIcon(":/resource/edit"),tr("Edit"),ptb);
    connect(this->saveAns,SIGNAL(triggered()),this,SLOT(on_editAns_triggered()));
    delAns = new QAction(QIcon(":/resource/erase"),tr("Del"),ptb);
    connect(this->delAns,SIGNAL(triggered()),this,SLOT(on_delAns_triggered()));
    ptb->addAction(addAns);
    ptb->addSeparator();
    ptb->addAction(saveAns);
    ptb->addSeparator();
    ptb->addAction(delAns);
    ui->gridLayout_Answers_tb->addWidget(ptb,0,0,0,2,Qt::AlignTop);
    ui->comboBox_Themes->clear();
    loadAnswers(answers_from_db);
    ui->comboBox_Type->addItem(tr("text entry"), 1);
    ui->comboBox_Type->addItem(tr("one correct answer"), 2);
    ui->comboBox_Type->addItem(tr("many correct answers"), 3);
    lastIndexOfAnswersType = ui->comboBox_Type->currentIndex();
    answersChecks.clear();
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
void question_mod_dialog::clear_AnswerList()
{
    for(int r=ui->tableWidget_Answers->rowCount(); r>0; r--){
        ui->tableWidget_Answers->removeRow(r-1);
    }
}
//
bool question_mod_dialog::getAnswerCheckAvailablity(bool oneCheckException)
{
    int answer_type = ui->comboBox_Type->currentData().toInt();
    bool result = false;
    if(answer_type == 3) {
        result = true;
    }
    else if(answer_type == 2){
        if(oneCheckException){
            result = true;
        }
        else{
            bool flag = true;
            for(int i=0; i<ui->tableWidget_Answers->rowCount();i++){
                if(answersChecks.at(i)->isChecked()){
                    flag = false;
                    break;
                }
            }

            if(flag){
                result = true;
            }
            else{
                result = false;
            }
        }
    }

    return result;
}
//
void question_mod_dialog::loadAnswers(QList<st_answer> *answers)
{

    clear_AnswerList();
    ui->tableWidget_Answers->setRowCount(answers->count());
    for(int a=0;a<answers->count();a++){
        ui->tableWidget_Answers->setItem(a,0,new QTableWidgetItem(answers->at(a).ans_text));
        ui->tableWidget_Answers->setItem(a,1,new QTableWidgetItem(answers->at(a).ans_correct));
    }
}
//
void question_mod_dialog::on_addAns_triggered()
{
    answer_mod_dlg *dlg = new answer_mod_dlg(getAnswerCheckAvailablity());
    dlg->setWindowTitle(tr("Add answer")+"...");
    if(dlg->exec()){
        if(dlg->getAnswerText().length() > 0){
            if( ui->tableWidget_Answers->findItems(dlg->getAnswerText(), Qt::MatchExactly).count() == 0)
            {
                int i = ui->tableWidget_Answers->rowCount();
                ui->tableWidget_Answers->insertRow(i);

                ui->tableWidget_Answers->setItem(i,0,new QTableWidgetItem(dlg->getAnswerText()));

                answersChecks.insert(i,new QCheckBox(this));
                answersChecks.at(i)->setChecked(dlg->getAnswerCorrectFlag());

                answersChecks.at(i)->setEnabled(false);
                QWidget *_wgt = new QWidget(this);
                QHBoxLayout *_hlw = new QHBoxLayout;
                _hlw->setMargin(0);
                _hlw->addWidget(answersChecks.at(i), 0, Qt::AlignCenter);
                _wgt->setLayout(_hlw);
                ui->tableWidget_Answers->setCellWidget(i,1,_wgt);
                ui->tableWidget_Answers->resizeColumnsToContents();

                if(ui->comboBox_Type->currentData().toInt() == 1){
                    addAns->setEnabled(false);
                    answersChecks.at(0)->setChecked(true);

                }
            }
            else
            {
                QString answerText = dlg->getAnswerText();
                if(answerText.length() > 30){
                    answerText = dlg->getAnswerText().left(20)+"....."+dlg->getAnswerText().right(10);
                }
                QMessageBox::critical(this,tr("Error"),tr("This answer ")+"\""+answerText+"\""+tr(" already exists!"));
            }
        }
    }
    delete dlg;
}
//
QVariant question_mod_dialog::getIndexBox()
{
    return ui->comboBox_Type->itemData(ui->comboBox_Type->currentIndex());
}
//
QString question_mod_dialog::getComment()
{
    return ui->textEdit_AnsComment->toPlainText().trimmed();
}
//
void question_mod_dialog::on_editAns_triggered()
{
    QTableWidgetItem *itm = ui->tableWidget_Answers->currentItem();
    if(itm){
        answer_mod_dlg *dlg = new answer_mod_dlg(getAnswerCheckAvailablity(answersChecks.at(itm->row())->isChecked()));
        dlg->setWindowTitle(tr("Edit answer")+"...");
        dlg->setAnswerText(ui->tableWidget_Answers->item(itm->row(),0)->text());
        dlg->setAnswerCorrectFlag(answersChecks.at(itm->row())->isChecked());
        if(dlg->exec()){
            if(dlg->getAnswerText().length() == 0){
                int ret = QMessageBox::question(this, tr("Edit answer"),
                                                tr("Answer text is empty.")+" \n\""+
                                                tr("Delete this answer")+"?",
                                                QMessageBox::Yes | QMessageBox::No,
                                                QMessageBox::No);
                if(ret == QMessageBox::Yes){
                    removeAnswer(itm->row(),true);
                }
            }
            else{
                ui->tableWidget_Answers->item(itm->row(),0)->setText(dlg->getAnswerText());
                answersChecks.at(itm->row())->setChecked(dlg->getAnswerCorrectFlag());
                ui->tableWidget_Answers->resizeColumnsToContents();
            }
        }
        delete dlg;
    }
    else{
        QMessageBox::information(this,tr("Information"),tr("Please, select answer for edit."));
    }
}
//
void question_mod_dialog::on_delAns_triggered()
{
    QTableWidgetItem *itm = ui->tableWidget_Answers->currentItem();
    if(itm){
        removeAnswer(itm->row());
        if(ui->comboBox_Type->currentData().toInt() == 1){
            addAns->setEnabled(true);
        }
    }
    else{
        QMessageBox::information(this,tr("Information"),tr("Please, select answer for remove."));
    }
}
//
int question_mod_dialog::correctAnswersCount(){
    int result = 0;
    for(int a = 0; a< answersChecks.count(); a++){
        if(answersChecks.at(a)->isChecked()){
            result++;
        }
    }
    return result;
}
//
void question_mod_dialog::on_comboBox_Type_currentIndexChanged(int index)
{
    if(ui->comboBox_Type->itemData(index) == 1){
        if(ui->tableWidget_Answers->rowCount() > 1){
            QTableWidgetItem *itm = ui->tableWidget_Answers->currentItem();
            if(itm){
                int ret = QMessageBox::question(this, tr("Type answer"),
                                                tr("You actually do want to remove all answers except that selected")+"?",
                                                QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
                if(ret == QMessageBox::Yes){
                    QString itm4save_text = ui->tableWidget_Answers->item(itm->row(),0)->text();
                    while(ui->tableWidget_Answers->rowCount() > 1){
                        int i = 0;
                        begin:
                        QTableWidgetItem *itm4del = ui->tableWidget_Answers->item(i,0);

                        if(itm4del->text() != itm4save_text){
                            removeAnswer(i,true);
                        }
                        else{
                            if(ui->tableWidget_Answers->rowCount() > 1){
                                i++;
                                goto begin;
                            }
                        }
                    }
                    if(ui->tableWidget_Answers->rowCount() > 0 ) addAns->setEnabled(false);
                    answersChecks.at(0)->setChecked(true);
                    lastIndexOfAnswersType = index;
                }
                else{
                    ui->comboBox_Type->setCurrentIndex(lastIndexOfAnswersType);
                }
            }
            else{
                QMessageBox::information(this,tr("Information"),tr("Please select one answer will be considered correct, prior to changing the type of response"));
                ui->comboBox_Type->setCurrentIndex(lastIndexOfAnswersType);
            }
        }
    }
    else if(ui->comboBox_Type->itemData(index) == 2 && correctAnswersCount() > 1){
        int ret = QMessageBox::question(this, tr("Change type of answers"),
                                        tr("The selected type of answers do not support multiple correct answers.")+" \n\""+
                                        tr("All flags will be dropped.")+"\n\n"+tr("Are you sure?"),
                                        QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if(ret == QMessageBox::Yes){
            for(int a = 0; a< answersChecks.count(); a++){
                answersChecks.at(a)->setChecked(false);
            }
            lastIndexOfAnswersType = index;
            if(!addAns->isEnabled()) addAns->setEnabled(true);
            QMessageBox::information(this,tr("Information"),tr("Please set correct answer."));
        }
        else{
            ui->comboBox_Type->setCurrentIndex(lastIndexOfAnswersType);
        }
    }
    else{
        if(!addAns->isEnabled()) addAns->setEnabled(true);
        lastIndexOfAnswersType = index;
    }
}
//
void question_mod_dialog::removeAnswer(int row, bool quiet)
{
    bool ready = false;
    if(!quiet){
        QTableWidgetItem *itm = ui->tableWidget_Answers->item(row,0);
        QString answerText = itm->text().trimmed();
        if(answerText.length() > 30){
            answerText = itm->text().trimmed().left(20)+"....."+itm->text().trimmed().right(10);
        }
        int ret = QMessageBox::question(this, tr("Remove answer..."),
                                        tr("Are you sure want delete answer:")+" \n\""+answerText+"\"?",
                                        QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if(ret == QMessageBox::Yes){
            ready = true;
        }
    }
    else{
        ready = true;
    }

    if(ready){
        answersChecks.removeAt(row);
        ui->tableWidget_Answers->removeRow(row);
    }

}
