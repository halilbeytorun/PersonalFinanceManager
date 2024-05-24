#include "entrancewindow.h"
#include "AuthenticationModule/AuthenticationModule.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    EntranceWindow w;
    w.show();
    return a.exec();
}
