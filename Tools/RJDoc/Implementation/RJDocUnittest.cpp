#include "gtest/gtest.h"
#include "RJDoc.h"

TEST(TestRJDoc, CaseMinimal)
{
  RJDoc reader("/home/ws/denso/daito/ProjComp/BTServices/Tools/RJDoc/Implementation/minimal.json"); // 1
  ASSERT_TRUE(reader.checkStatus()); // 2
  ASSERT_EQ(419, reader.getTotal()); // 3
  ASSERT_EQ(20, reader.getNumber());
 
  const rapidjson::Value& ds = reader.getDs(); // 4
  ASSERT_EQ((unsigned int)2, ds.Size()); // 5
 
  ASSERT_STREQ("one/a", ds[0U]["a"].GetString()); // 6
  ASSERT_STREQ("two/b", ds[1U]["b"].GetString());
  ASSERT_THROW(ds[3U]["a"].GetString(), std::logic_error); // 7
}