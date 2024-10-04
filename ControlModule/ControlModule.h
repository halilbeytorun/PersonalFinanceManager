#ifndef ControlModule_H
#define ControlModule_H

#include "UIModule/stackedwindows.h"
#include "AuthenticationModule/AuthenticationModule.h"
#include "DataManagementModule/DataManagementModule.h"

#include <QApplication>

class ControlModule
{
public:
    ControlModule(int& arc, char* argv[]);
    void Init();
    void run();
    bool pushButtonRPRegister_clicked(const std::string& user_name, const std::string& passport);
    bool pushButtonEPOk_clicked(const std::string& user_name, std::string& passport);
private:
    QApplication application_;
    DataManagementModule data_management_module_;
    StackedWindows stacked_windows_;
    AuthenticationModule authentication_module_;
};

#endif  // ControlModule_H
