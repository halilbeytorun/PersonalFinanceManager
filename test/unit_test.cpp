#include <gtest/gtest.h>
#include "../DataManagementModule/DataManagementModule.h"

class ProxyClient : public testing::Test
{
protected:
  void SetUp() override
  {

  }
};

#include <sqlite3.h>

TEST_F(ProxyClient, DataManagementModuleTest)
{
  std::string test_db_name{"./test.db"};
  DataManagementModule test;
  int return_code{};
  return_code = test.InitializeDB(test_db_name);
  ASSERT_EQ(return_code, SQLITE_OK);
}

