#ifndef AuthenticationModule_H
#define AuthenticationModule_H

#include <string>
#include <utility>

class DataManagementModule;
class AuthenticationModule
{
public:
    AuthenticationModule(DataManagementModule& module);
    std::string login_table_name() const;

    void set_user_name_passport(const std::pair<std::string, std::string>& user_name_passport);
    std::pair<std::string, std::string> user_name_passport() const;
private:
    DataManagementModule& m_DataManagementModule;
    std::string login_table_name_;
    std::pair<std::string, std::string> user_name_passport_;
};

#endif
