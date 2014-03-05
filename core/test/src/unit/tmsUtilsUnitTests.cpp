#include <boost/test/unit_test.hpp>
#include <string>
#include "tmsUtils.hpp"
#include "UserException.hpp"
#include "constants.hpp"

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
  const long defaultTime(21600);
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


  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(""), defaultTime);

  long reference = defaultTime + 10;
  // Only seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10b), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10a), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall10ab), reference);

  reference = defaultTime + 670;
  // minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670b), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670a), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall670ab), reference);

  reference = defaultTime + 43870;
  // hours:minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870b), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870a), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall43870ab), reference);

  reference = defaultTime + 11535070;
  // days:hours:minutes:seconds
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070b), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070a), reference);
  BOOST_REQUIRE_EQUAL(vishnu::convertStringToWallTime(wall11535070ab), reference);
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

BOOST_AUTO_TEST_CASE( test_convertJobStateToString_n )
{
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_UNDEFINED), "UNDEFINED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_SUBMITTED), "SUBMITTED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_QUEUED), "QUEUED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_WAITING), "WAITING");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_RUNNING), "RUNNING");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_COMPLETED), "COMPLETED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_CANCELLED), "CANCELLED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_DOWNLOADED), "DOWNLOADED");
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_FAILED), "FAILED");
}

BOOST_AUTO_TEST_CASE( test_convertJobStateToString_b )
{
  BOOST_REQUIRE_EQUAL(vishnu::convertJobStateToString(vishnu::STATE_FAILED + 10), "UNDEFINED");
}

BOOST_AUTO_TEST_CASE( test_convertWallTimeToString_n )
{
  long one_second  = 1;
  long ten_seconds = 10;
  long one_minute  = 60;
  long ten_minutes = one_minute * 10;
  long one_hour    = one_minute * 60;
  long ten_hours   = one_hour * 10;
  long one_day     = one_hour * 24;
  long ten_days    = one_day * 10;

  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(one_second), "00:00:01");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(ten_seconds), "00:00:10");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(one_minute), "00:01:00");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(ten_minutes), "00:10:00");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(one_hour), "01:00:00");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(ten_hours), "10:00:00");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(one_day), "01:00:00:00");
  BOOST_REQUIRE_EQUAL(vishnu::convertWallTimeToString(ten_days), "10:00:00:00");
}


BOOST_AUTO_TEST_SUITE_END()
