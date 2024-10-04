#include "DataManagementModule.h"
#include "hutils.h"


#include <sqlite3.h>


ReturnCode DataManagementModule::InitializeDB(const std::string& db_loc)
{
    int return_code{};
    return_code = sqlite3_open(db_loc.c_str(), &m_db_);

    if(SQLITE_OK == return_code)
    {
        return ReturnCode::Ok;
    }
    else
    {
        Logger(static_cast<const char*>(sqlite3_errmsg(m_db_)), " return code:", return_code);
        return ReturnCode::DataBaseCreationError;
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

ReturnCode DataManagementModule::CreateLoginTable(const std::string& table_name)
{
    std::string sql = std::string{"CREATE TABLE IF NOT EXISTS "} + table_name + std::string{" (username VARCHAR(50) NOT NULL,passport VARCHAR(20) NOT NULL,PRIMARY KEY (username));"};
    char* error_message{};
    int return_code{};
    sqlite3_stmt* statement{};
    
    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);

    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::TableCreationError;
    }

    return_code = sqlite3_step(statement);

    if(SQLITE_DONE != return_code)
    {
        PrintError(m_db_, return_code);
    }
    return_code = sqlite3_finalize(statement);
    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
    }
    return ReturnCode::Ok;
}

ReturnCode DataManagementModule::DeleteLoginTable(const std::string& table_name)
{
    std::string sql = std::string{"DROP TABLE IF EXISTS "} + table_name + std::string{";"};
    char* error_message{};
    int return_code{};
    sqlite3_stmt* statement{};

    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);

    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::TableDeletionError;
    }

    return_code = sqlite3_step(statement);

    if(SQLITE_DONE != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::TableDeletionError;
    }

    return_code = sqlite3_finalize(statement);

    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::TableDeletionError;
    }

    return ReturnCode::Ok;
}


ReturnCode DataManagementModule::TableExists(const std::string& table_name)
{
    std::string sql{"SELECT name FROM sqlite_master WHERE type='table' AND name=?;"};
    sqlite3_stmt* statement{};
    int return_code{};

    // Prepare the SQL statement
    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);

    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::TableCheckError;
    }

    // Bind the table name to the placeholder (?)
    return_code = sqlite3_bind_text(statement, 1, table_name.c_str(), -1, SQLITE_STATIC);

    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::TableCheckError;
    }

    // Execute the statement and check if the table exists
    return_code = sqlite3_step(statement);
    
    if (return_code == SQLITE_ROW)
    {
        // Table exists
        sqlite3_finalize(statement);
        return ReturnCode::TableExists;
    }
    else if (return_code == SQLITE_DONE)
    {
        // Table does not exist
        sqlite3_finalize(statement);
        return ReturnCode::TableDoesNotExist;
    }
    else
    {
        // Some other error occurred
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::TableCheckError;
    }
}


ReturnCode DataManagementModule::InsertIntoLoginTable(const std::string& table_name, const std::string& user_name, const std::string& passport, bool do_nothing_if_exists)
{
    std::string sql{};
    sqlite3_stmt* statement{};
    int return_code{};

    // Define the SQL query depending on the conflict handling behavior
    if (do_nothing_if_exists)
    {
        sql = std::string{"INSERT INTO "} + table_name + std::string{" (username, passport) VALUES (?1, ?2) ON CONFLICT (username) DO NOTHING;"};
    }
    else
    {
        sql = std::string{"INSERT INTO "} + table_name + std::string{" (username, passport) VALUES (?1, ?2);"};
    }

    // Prepare the SQL statement
    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::StatementPreparationError;
    }

    // Bind the username and passport values to the prepared statement
    return_code = sqlite3_bind_text(statement, 1, user_name.c_str(), -1, SQLITE_STATIC);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::BindingError;
    }

    return_code = sqlite3_bind_text(statement, 2, passport.c_str(), -1, SQLITE_STATIC);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::BindingError;
    }

    // Execute the statement (insert data)
    return_code = sqlite3_step(statement);
    if (SQLITE_DONE != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::ExecutionError;
    }

    // Finalize the statement (clean up)
    return_code = sqlite3_finalize(statement);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::FinalizationError;
    }

    return ReturnCode::Ok;
}


#include <iostream>
ReturnCode DataManagementModule::SelectRowFromLoginTable(const std::string& table_name, const std::string& user_name, std::string& passport)
{
    std::string sql = std::string{"SELECT passport FROM "} + table_name + std::string{" WHERE username = ?;"};
    sqlite3_stmt* statement{};
    int return_code{};

    // Prepare the SQL statement
    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::StatementPreparationError;
    }

    // Bind the username value to the prepared statement
    return_code = sqlite3_bind_text(statement, 1, user_name.c_str(), -1, SQLITE_STATIC);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::BindingError;
    }

    // Execute the statement and retrieve the data
    return_code = sqlite3_step(statement);
    if (return_code == SQLITE_ROW)
    {
        // Extract the passport value from the result
        const unsigned char* passport_value = sqlite3_column_text(statement, 0);
        if (passport_value)
        {
            passport = reinterpret_cast<const char*>(passport_value);
        }
    }
    else if (return_code == SQLITE_DONE)
    {
        // No row was found for the given username
        sqlite3_finalize(statement);
        return ReturnCode::RowNotFound;
    }
    else
    {
        // Some other error occurred
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::ExecutionError;
    }

    // Finalize the statement to clean up resources
    return_code = sqlite3_finalize(statement);
    if (SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::FinalizationError;
    }

    return ReturnCode::Ok;
}


ReturnCode DataManagementModule::DeleteFromLoginTable(const std::string& table_name, const std::string& user_name)
{
    int return_code{};
    sqlite3_stmt* statement{};
    std::string sql = std::string{"DELETE from "} + table_name + std::string{" WHERE username = ?1;"};
    
    return_code = sqlite3_prepare_v2(m_db_, sql.c_str(), -1, &statement, nullptr);
    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::StatementPreparationError;
    }

    return_code = sqlite3_bind_text(statement, 1, user_name.c_str(), -1, SQLITE_STATIC);
    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::BindingError;
    }

    return_code = sqlite3_step(statement);
    if(SQLITE_DONE != return_code)
    {
        PrintError(m_db_, return_code);
        sqlite3_finalize(statement);
        return ReturnCode::ExecutionError;
    }

    return_code = sqlite3_finalize(statement);
    if(SQLITE_OK != return_code)
    {
        PrintError(m_db_, return_code);
        return ReturnCode::FinalizationError;
    }
    return ReturnCode::Ok;
}


DataManagementModule::~DataManagementModule()
{
    if(m_db_)
    {
        sqlite3_close(m_db_);
    }
}


void DataManagementModule::PrintError(const sqlite3* m_db, int return_code)
{
    Logger(static_cast<const char*>(sqlite3_errmsg(m_db_)), " return code:", return_code);
}
