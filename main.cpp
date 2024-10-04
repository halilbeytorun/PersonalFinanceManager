#include "ControlModule/ControlModule.h"

int main(int argc, char *argv[])
{

     // QApplication a(argc, argv);
     // StackedWindows w;
     // w.show();
     // return a.exec();

     ControlModule control_module(argc, argv);
     control_module.Init();
     control_module.run();
}
