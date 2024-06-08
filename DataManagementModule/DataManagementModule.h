#ifndef DataManagementModule_H
#define DataManagementModule_H

#include <string>


struct sqlite3;

/// @brief Deals with the storage and retrieval of financial data, user data. Uses SQLite database to store related data.
class DataManagementModule
{
public:
    int InitializeDB(const std::string&);
    int CreateLoginTable();
    int InsertLoginTable(const std::string& user_name, const std::string& passport);
    int SelectRowLoginTable(const std::string& user_name, std::string& passport);
    void InsertQuery();
    ~DataManagementModule();

private:
    sqlite3* m_db{};

};

#endif
