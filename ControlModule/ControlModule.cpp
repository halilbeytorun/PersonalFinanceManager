#include "ControlModule.h"


#include <string>


ControlModule::ControlModule(int& argc, char *argv[]) : 
                            application_{argc, argv},
                            data_management_module_{}, 
                            stacked_windows_{*this}, 
                            authentication_module_{data_management_module_}
{
}

void ControlModule::Init()
{
    auto return_code = data_management_module_.InitializeDB("/home/halil/workspace/PersonalFinanceManager/SQlitedatabase.db");
    if(return_code != ReturnCode::Ok)
        throw std::domain_error{"Database initialization error"};
    return_code = data_management_module_.CreateLoginTable(authentication_module_.login_table_name());
    if(return_code != ReturnCode::Ok)
        throw std::domain_error{"Database table creation error"};
}


void ControlModule::run()
{
    stacked_windows_.show();
    application_.exec();
}

bool ControlModule::pushButtonRPRegister_clicked(const std::string& user_name, const std::string& passport)
{
    auto return_code = data_management_module_.InsertIntoLoginTable(authentication_module_.login_table_name(), user_name, passport);
    return return_code == ReturnCode::Ok;
}

bool ControlModule::pushButtonEPOk_clicked(const std::string& user_name, std::string& passport)
{
    auto return_code = data_management_module_.SelectRowFromLoginTable(authentication_module_.login_table_name(), user_name, passport);
    return return_code == ReturnCode::Ok;
}


