#include <boost/test/unit_test.hpp>
#include <string>
#include "tmsUtils.hpp"

BOOST_AUTO_TEST_SUITE( tmsUtils_unit_tests )

BOOST_AUTO_TEST_CASE( test_checkJobStatus_n )
{
  int input_1 = 1;
  int input_2 = -2;
  int input_3 = 9;

  vishnu::checkJobStatus(input_1);
  BOOST_REQUIRE_THROW(vishnu::checkJobStatus(input_2), UserException);
  BOOST_REQUIRE_THROW(vishnu::checkJobStatus(input_3), UserException);
  BOOST_MESSAGE("Test check job status OK");
}

BOOST_AUTO_TEST_CASE( test_checkJobPrio_n )
{
  int input_1 = 1;
  int input_2 = -2;
  int input_3 = 9;

  vishnu::checkJobPriority(input_1);
  BOOST_REQUIRE_THROW(vishnu::checkJobPriority(input_2), UserException);
  BOOST_REQUIRE_THROW(vishnu::checkJobPriority(input_3), UserException);
  BOOST_MESSAGE("Test check job priority OK");
}

// TODO move
BOOST_AUTO_TEST_CASE( test_checkJobnbnodecpu_n )
{
  std::string input_1 = "1:1";
  std::string input_2 = "-2";
  std::string input_3 = "3:a7";
  std::string input_4 = "";

  vishnu::checkJobNbNodesAndNbCpuPerNode(input_1);
  vishnu::checkJobNbNodesAndNbCpuPerNode(input_4);
  BOOST_REQUIRE_THROW(vishnu::checkJobNbNodesAndNbCpuPerNode(input_2), UserException);
  BOOST_REQUIRE_THROW(vishnu::checkJobNbNodesAndNbCpuPerNode(input_3), UserException);
  BOOST_MESSAGE("Test check job nb node cpu OK");
}


BOOST_AUTO_TEST_CASE( test_convertStringToWallTime_n )
{
  std::string wall10 = "10";
  std::string wall10b = "\"10";
  std::string wall10a = "10\"";
  std::string wall10ab = "\"10\"";
  std::string wall670 = "11:10";
  std::string wall670b = "\"11:10";
  std::string wall670a = "11:10\"";
  std::string wall670ab = "\"11:10\"";
  std::string wall43870 = "12:11:10";
  std::string wall43870b = "\"12:11:10";
  std::string wall43870a = "12:11:10\"";
  std::string wall43870ab = "\"12:11:10\"";
  std::string wall11535070 = "133:12:11:10";
  std::string wall11535070b = "\"133:12:11:10";
  std::string wall11535070a = "133:12:11:10\"";
  std::string wall11535070ab = "\"133:12:11:10\"";


  // Only seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10), 10);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10b), 10);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10a), 10);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10ab), 10);

  // minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670), 670);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670b), 670);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670a), 670);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670ab), 670);

  // hours:minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870), 43870);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870b), 43870);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870a), 43870);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870ab), 43870);

  // days:hours:minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070), 11535070);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070b), 11535070);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070a), 11535070);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070ab), 11535070);
}

BOOST_AUTO_TEST_CASE( test_convertStringToWallTime_b )
{
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("bad"), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("12:133:12:11:10"), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("\"12:133:12:11:10"), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("12:133:12:11:10\""), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("\"12:133:12:11:10\""), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime(""), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("\""), UserException);
  BOOST_REQUIRE_THROW(vishnu::convertStringToWallTime("\"\""), UserException);
}


BOOST_AUTO_TEST_CASE( test_convertToBatchType_n )
{
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("TORQUE"), TORQUE);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("SLURM"), SLURM);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("LOADLEVELER"), LOADLEVELER);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("LSF"), LSF);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("SGE"), SGE);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("DELTACLOUD"), DELTACLOUD);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("PBS"), PBSPRO);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("POSIX"), POSIX);
}

BOOST_AUTO_TEST_CASE( test_convertToBatchType_b )
{
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType(""), UNDEFINED);
  BOOST_REQUIRE_EQUAL(vishnu::convertToBatchType("pouet"), UNDEFINED);
}


BOOST_AUTO_TEST_SUITE_END()
