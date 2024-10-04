#ifndef DataManagementModule_H
#define DataManagementModule_H

#include <string>


struct sqlite3;

/// @brief ReturnCode of DataManagement Module
enum class ReturnCode
{
    Ok,
    DataBaseCreationError,
    TableCreationError,
    TableDeletionError,
    TableCheckError,
    TableDoesNotExist,
    TableExists,
    StatementPreparationError,
    BindingError,
    ExecutionError,
    FinalizationError,
    RowNotFound,   
    Error
};

/// @brief Deals with the storage and retrieval of financial data, user data. Uses SQLite database to store related data.
class DataManagementModule
{
public:
    ReturnCode InitializeDB(const std::string&);
    ReturnCode CreateLoginTable(const std::string& table_name);
    ReturnCode DeleteLoginTable(const std::string& table_name);
    ReturnCode InsertIntoLoginTable(const std::string& table_name, const std::string& user_name, const std::string& passport, bool do_nothing_if_exists = false);
    ReturnCode SelectRowFromLoginTable(const std::string& table_name, const std::string& user_name, std::string& passport);
    ReturnCode DeleteFromLoginTable(const std::string& table_name, const std::string& user_name);
    ReturnCode TableExists(const std::string& table_name);
    ~DataManagementModule();
private:
    sqlite3* m_db_{};
    void PrintError(const sqlite3* m_db, int return_code);
};

#endif //DataManagementModule_H
