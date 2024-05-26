#include "DataManagementModule.h"


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
        sqlite3_errmsg16(m_db);
    }
}

DataManagementModule::~DataManagementModule()
{
    if(m_db)
    {
        sqlite3_close(m_db);
    }
}
