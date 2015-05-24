#ifndef PUB_STRUCT_H
#define PUB_STRUCT_H

#include <QString>
#include <QList>

struct st_group{
    QString id;
    QString code;
};

struct st_theme{
    QString id;
    QString parent_id;
    QString name;
};

struct st_quesion{
    QString id;
    QString theme_id;
    QString text;
    QString comment;
    int ans_type;
};

struct st_answer{
    QString question_id;
    QString ans_id;
    QString ans_text;
    bool ans_correct;
};

struct st_recipient{
    QString id;
    QString name;
    QString address;
};

struct st_email{
    QList<st_recipient> recipient;
    QString msg_subj;
    QString msg_body;

};

struct st_smtp{
    QString server;
    int port;
    QString mail_from;
    QString username;
    QString password;
    bool ssl;
};
#endif // PUB_STRUCT_H

