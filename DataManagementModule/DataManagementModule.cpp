#include "DataManagementModule.h"
#include "hutils.h"


#include <sqlite3.h>


int DataManagementModule::InitializeDB(const std::string& db_loc)
{
    int return_code{};
    return_code = sqlite3_open(db_loc.c_str(), &m_db);

    if(SQLITE_OK == return_code)
    {
        return return_code;
    }
    else
    {
        Logger(static_cast<const char*>(sqlite3_errmsg(m_db)));
        return return_code;
    }
}

/*
CREATE TABLE users (
    username VARCHAR(50) NOT NULL,
    passport VARCHAR(20) NOT NULL,
    PRIMARY KEY (username)

CREATE TABLE IF NOT EXISTS users (
    username VARCHAR(50) NOT NULL,
    passport VARCHAR(20) NOT NULL,
    PRIMARY KEY (username)
);
*/

int DataManagementModule::CreateLoginTable()
{
    std::string sql{"CREATE TABLE IF NOT EXISTS users (username VARCHAR(50) NOT NULL,passport VARCHAR(20) NOT NULL,PRIMARY KEY (username));"};
    char* error{};
    int return_code{};
    return_code = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &error);

    if(SQLITE_OK == return_code)
    {
        return return_code;
    }
    else
    {
        if(error)
        {
            sqlite3_free(error);
        }
        Logger(static_cast<const char*>(sqlite3_errmsg(m_db)));
        return return_code;
    }
}

int DataManagementModule::InsertLoginTable(const std::string& user_name, const std::string& passport)
{
    // INSERT INTO users (username, passport) VALUES ('your_username', 'your_passport');
    std::string sql{"INSERT INTO users (username, passport) VALUES ('" + user_name + "', '" + passport + "');"};

    char* error{};
    int return_code{};
    return_code = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &error);

    if(SQLITE_OK == return_code)
    {
        return return_code;
    }
    else
    {
        if(error)
        {
            sqlite3_free(error);
        }
        Logger(static_cast<const char*>(sqlite3_errmsg(m_db)));        
        return return_code;
    }
}
#include <iostream>
int DataManagementModule::SelectRowLoginTable(const std::string& user_name, std::string& passport)
{
    // SELECT username, passport FROM users WHERE username = 'your_username';
    std::string sql{"SELECT username, passport FROM users WHERE username = '" + user_name + "';"};

    auto callback = [] (void * passport, int argc, char ** colmn_number, char ** colmn_name) { 
        static_cast<std::string*>(passport)->operator=(colmn_number[1]);
        for(int i = 0; i < argc; i++)
        {
            std::cout << colmn_number[i] << " ";
        }
        std::cout << "\n";
        for(int i = 0; i < argc; i++)
        {
            std::cout << colmn_name[i] << " ";
        }
        std::cout << "\n";
        return 0;};

    char* error{};
    int return_code{};
    return_code = sqlite3_exec(m_db, sql.c_str(), callback, &passport, &error);

    if(SQLITE_OK == return_code)
    {
        return return_code;
    }
    else
    {
        if(error)
        {
            sqlite3_free(error);
        }
        sqlite3_errmsg(m_db);
        return return_code;
    }

}


DataManagementModule::~DataManagementModule()
{
    if(m_db)
    {
        sqlite3_close(m_db);
    }
}
