#include <QtGui/QApplication>
#include "simon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    simon w;
    w.show();
    return a.exec();
}
