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

  return_code = test.CreateLoginTable();
  ASSERT_EQ(return_code, SQLITE_OK);

  // todo handle more than once insert case.. TODO
  std::string user_name{"halil"};
  std::string passport{"123"};
  // return_code = test.InsertLoginTable(user_name, passport);
  // ASSERT_EQ(return_code, SQLITE_OK);

  std::string potantial_passport{};
  return_code = test.SelectRowLoginTable(user_name, potantial_passport);
  ASSERT_EQ(passport, potantial_passport);

  std::string wrong_user_name{"FALSE"};
  std::string wrong_passport{"FALSE"};
  std::string wrong_potantial_passport{};
  return_code = test.SelectRowLoginTable(wrong_user_name, wrong_potantial_passport);
  ASSERT_EQ(wrong_potantial_passport, std::string{});
}

