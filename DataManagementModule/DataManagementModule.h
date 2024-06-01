#ifndef DataManagementModule_H
#define DataManagementModule_H

#include <string>


struct sqlite3;

/// @brief Deals with the storage and retrieval of financial data, user data. Uses SQLite database to store related data.
class DataManagementModule
{
public:
    int InitializeDB(const std::string&);
    void InsertQuery();
    ~DataManagementModule();

private:
    sqlite3* m_db{};

};

#endif
