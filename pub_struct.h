#ifndef PUB_STRUCT_H
#define PUB_STRUCT_H

#include <QString>

struct st_theme{
    QString id;
    QString parent_id;
    QString name;
};

struct st_answer{
    QString question_id;
    QString ans_id;
    QString ans_text;
    bool ans_correct;
};

struct st_email{
    QString id;
    QString recipient_name;
    QString recipient_address;
    QString sender_name;
    QString sender_address;
    QString msg_subj;
    QString msg_body;

};

struct st_smtp{
    QString server;
    int port;
    QString username;
    QString password;
    bool ssl;
};
#endif // PUB_STRUCT_H

