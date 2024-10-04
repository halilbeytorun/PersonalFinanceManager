#include "AuthenticationModule.h"
#include "DataManagementModule/DataManagementModule.h"

#include <stdexcept>

AuthenticationModule::AuthenticationModule(DataManagementModule& module) : m_DataManagementModule(module), login_table_name_{"users"} {}


std::string AuthenticationModule::login_table_name()
{
    return login_table_name_;
}
