#include "UIModule/stackedwindows.h"
#include "AuthenticationModule/AuthenticationModule.h"

#include <QApplication>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{

     QApplication a(argc, argv);
     StackedWindows w;
     w.show();
     return a.exec();
}
