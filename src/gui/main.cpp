#include "Players.h"

#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Players window;
    window.show();
    return app.exec();
}
