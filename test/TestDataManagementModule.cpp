#include <gtest/gtest.h>
#include "../DataManagementModule/DataManagementModule.h"



// TODO: Think about value-parametrized tests..
class DataManagementModuleFixture : public testing::Test
{
protected:
  void SetUp() override
  {
    auto return_code = db_module_.InitializeDB("./testingDB.db");
    ASSERT_EQ(return_code, ReturnCode::Ok);
  }
  void TearDown() override
  {
    
  }
public:
  DataManagementModule db_module_;
};

#include <sqlite3.h>


TEST_F(DataManagementModuleFixture, CreateDeleteTable)
{
  const std::string table_name{"test_table"};
  auto return_code = db_module_.CreateLoginTable(table_name);
  ASSERT_EQ(return_code, ReturnCode::Ok);
  return_code = db_module_.TableExists(table_name);
  ASSERT_EQ(return_code, ReturnCode::TableExists);
  return_code = db_module_.DeleteLoginTable(table_name);
  ASSERT_EQ(return_code, ReturnCode::Ok);
}


TEST_F(DataManagementModuleFixture, TableInsert)
{
  const std::string table_name{"test_table"};
  auto return_code = db_module_.CreateLoginTable(table_name);
  ASSERT_EQ(return_code, ReturnCode::Ok);

  const std::string user_name{"halil"};
  const std::string passport{"123"};
  
  return_code = db_module_.InsertIntoLoginTable(table_name, user_name, passport);
  ASSERT_EQ(return_code, ReturnCode::Ok);

  return_code = db_module_.InsertIntoLoginTable(table_name, user_name, passport, true); // do nothing
  ASSERT_EQ(return_code, ReturnCode::Ok);

  std::string expected_passport{};
  return_code = db_module_.SelectRowFromLoginTable(table_name, user_name, expected_passport);
  ASSERT_EQ(return_code, ReturnCode::Ok);
  ASSERT_EQ(passport, expected_passport);

  std::string wrong_user_name{"FALSE"};
  std::string wrong_passport{"FALSE"};
  std::string wrong_expected_passport{};
  return_code = db_module_.SelectRowFromLoginTable(table_name, wrong_user_name, wrong_expected_passport);
  ASSERT_EQ(return_code, ReturnCode::RowNotFound);
  ASSERT_NE(wrong_expected_passport, wrong_passport);

  return_code = db_module_.DeleteFromLoginTable(table_name, user_name);
  ASSERT_EQ(return_code, ReturnCode::Ok);

  std::string expected_passport2{};
  return_code = db_module_.SelectRowFromLoginTable(table_name, user_name, expected_passport2);
  ASSERT_EQ(return_code, ReturnCode::RowNotFound);
  ASSERT_NE(passport, expected_passport2);


  return_code = db_module_.DeleteLoginTable(table_name);
  ASSERT_EQ(return_code, ReturnCode::Ok);
}

