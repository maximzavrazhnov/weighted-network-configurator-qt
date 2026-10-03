#include "Players.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Players w;
    w.show();
    return a.exec();
}
