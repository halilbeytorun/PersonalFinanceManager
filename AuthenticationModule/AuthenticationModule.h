#ifndef AuthenticationModule_H
#define AuthenticationModule_H

#include <string>
#include <utility>
#include <map>
#include <vector>
#include <tuple>

class DataManagementModule;
class AuthenticationModule
{
public:
    AuthenticationModule(DataManagementModule& module);
    std::string login_table_name() const;

    void set_user_name_passport(const std::pair<std::string, std::string>& user_name_passport);
    std::pair<std::string, std::string> user_name_passport() const;

    struct UserInfo
    {
        double monthlyIncome{};
        /// @brief Spent category, strongi every category (including also UnUsed category by default). The value left inside of the category 
        /// and inside categories are also designed to be stored
        std::map<std::string, std::tuple<double, double, std::map<std::string,double>>> spent_category;
        // std::map<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>> spent_category;
    };
private:
    DataManagementModule& m_DataManagementModule;
    std::string login_table_name_;
    std::pair<std::string, std::string> user_name_passport_;

};

#endif
