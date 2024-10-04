#include "ControlModule/ControlModule.h"

int main(int argc, char *argv[])
{
     ControlModule control_module(argc, argv);
     control_module.Init();
     control_module.run();
}
