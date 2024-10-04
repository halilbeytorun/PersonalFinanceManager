#ifndef AuthenticationModule_H
#define AuthenticationModule_H

#include <string>

class DataManagementModule;
class AuthenticationModule
{
public:
    AuthenticationModule(DataManagementModule& module);
    std::string login_table_name();
    // int Init();
private:
    DataManagementModule& m_DataManagementModule;
    std::string login_table_name_;
};

#endif
