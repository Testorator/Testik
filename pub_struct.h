#ifndef PUB_STRUCT_H
#define PUB_STRUCT_H

#include <QString>

struct st_answer{
    QString question_id;
    QString ans_id;
    QString ans_text;
    QString ans_comment;
    bool ans_correct;
};

#endif // PUB_STRUCT_H

