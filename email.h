#ifndef EMAIL_H
#define EMAIL_H

#include <QObject>

class email : public QObject
{
    Q_OBJECT
public:
    explicit email(QObject *parent = 0);
    ~email();

signals:

public slots:
};

#endif // EMAIL_H
