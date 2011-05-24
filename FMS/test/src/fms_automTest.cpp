/**
 * \file fms_automTest
 * \brief Contains FMS API test implementation
 * References: VISHNU_D3_1b_FMS-PlanTests
 */


//UMS forward Headers
#include "UMS_Data_forward.hpp"

// FMS forward Headers
#include "FMS_Data_forward.hpp"
#include "FMS_fixtures.hpp"
#include "FMS_testconfig.h"

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include "vishnuTestUtils.hpp"

// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace FMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

static const string adminId = "admin_1";
static const string adminPwd = "admin";
static const string userId = "user_1";
static const string userPwd = "toto";
static const string machineId1 = "machine_1";
static const string machineId2 = "machine_2";

// The database, UMS and FMS SeD are launched by FMSSedFixture.
BOOST_GLOBAL_FIXTURE(FMSSeDFixture)

// All tests of category 1,2,3 for FILES only
BOOST_AUTO_TEST_SUITE(singleFile)


BOOST_AUTO_TEST_CASE(CreateFile_Base)
{

  BOOST_TEST_MESSAGE("Testing file creation UC F1");

  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    const string localFilePath = "FMS_test_file";
    string fullFilePath = machineId1;
    fullFilePath.append(":" + localFilePath);
    BOOST_REQUIRE( createFile(sessionKey, fullFilePath) == 0);

    // To check the success of createFile function
    FileStat fileStat;
    BOOST_REQUIRE( getFilesInfo(sessionKey, fullFilePath, fileStat) ==0  );

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()


// THE END

