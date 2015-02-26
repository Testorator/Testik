#include "question_mod_dialog.h"
#include "ui_question_mod_dialog.h"

#include <QToolBar>

question_mod_dialog::question_mod_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::question_mod_dialog)
{
    ui->setupUi(this);
    QToolBar* ptb = new QToolBar("answers toolbar");
    ptb->addAction(QIcon(":/resourse/add"),tr("Add"),SLOT());
    ptb->setIconSize(QSize(24, 24));
    ptb->addSeparator();
    ptb->addAction(QIcon(":/resourse/save"),tr("Save"),SLOT());
    ptb->addSeparator();
    ptb->addAction(QIcon(":/resourse/del"),tr("Del"),SLOT());
    ui->gridLayout_Answers_tb->addWidget(ptb,0,0,0,2,Qt::AlignTop);



    return ptb;

}

question_mod_dialog::~question_mod_dialog()
{
    delete ui;
}
