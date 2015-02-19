#include "maxtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Maxtest w;

    w.show();

    return a.exec();
}
