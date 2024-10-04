#include "AuthenticationModule.h"
#include "DataManagementModule/DataManagementModule.h"

#include <stdexcept>

AuthenticationModule::AuthenticationModule(DataManagementModule& module) : m_DataManagementModule(module), login_table_name_{"users"} {}


std::string AuthenticationModule::login_table_name() const
{
    return login_table_name_;
}

std::pair<std::string, std::string> AuthenticationModule::user_name_passport() const
{
    return user_name_passport_;
}

void AuthenticationModule::set_user_name_passport(const std::pair<std::string, std::string>& user_name_passport)
{
    user_name_passport_ = user_name_passport;
}
