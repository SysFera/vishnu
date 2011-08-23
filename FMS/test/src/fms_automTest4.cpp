/**
 * \file fms_automTest
 * \brief Contains FMS API test implementation
 * References: VISHNU_D3_1b_FMS-PlanTests
 */
// Prerequisites for this test suite to work correctly:
// 1/ check the cmake parameters TEST_*
// 2/ setup a 'test' unix group on host TEST_FMS_HOST1
//    and add the user TEST_USER_LOGIN to this group
// 3/ check that the /root directory is not accessible for
//    the user that runs the tests (on TEST_FMS_HOST1)

//UMS forward Headers
#include "UMS_Data_forward.hpp"

// FMS forward Headers
#include "FMS_Data_forward.hpp"
#include "FMS_fixtures.hpp"
#include "FMS_testconfig.h"
#define STATUS_INPROGRESS 0
#define STATUS_COMPLETED 1
#define STATUS_CANCELED 2
#define STATUS_FAILED 3

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp>

// Vishnu Headers

#include "utilVishnu.hpp"
#include "vishnuTestUtils.hpp"

// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace FMS_Data;
using namespace vishnu;
namespace ba = boost::assign;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

static const string adminId = "admin_1";
static const string adminPwd = "admin";
static const string userId = "user_1";
static const string userPwd = "toto";
static const string userLogin = FMSUSERLOGIN;
static const string groupTest = "test"; // userLogin must belong to groupTest on host 1
static const string sep = ":";
static const string slash = "/";
static const string machineId1 = "machine_1"; // name is TEST_FMS_HOST1 set in cmake
static const string machineId2 = "machine_2"; // name is TEST_FMS_HOST2 set in cmake
static const string newFileName = "FMS_test_file";
static const string newDirName = "FMS_test_dir";
// local
static const string localDir = FMSWORKINGDIR;
static const string localFilePath = localDir + slash + newFileName;
// remote
static const string remoteBaseDir1 = FMSDIR1;
static const string remoteBaseDir2 = FMSDIR2;
static const string baseDirFullPath1 = machineId1 + sep + remoteBaseDir1;
static const string baseDirFullPath2 = machineId2 + sep + remoteBaseDir2;
static const string fileFullPath1 = baseDirFullPath1 + slash + newFileName;
static const string fileFullPath2 = baseDirFullPath2 + slash + newFileName;
static const string dirFullPath1 = baseDirFullPath1 + slash + newDirName;
static const string dirFullPath2 = baseDirFullPath2 + slash + newDirName;

// The database, UMS and FMS SeD are launched by FMSSedFixture.
BOOST_FIXTURE_TEST_SUITE(TailOfFile, FMSSeDFixture)

/*****************************************************************************/
/******************************* F I L E S ***********************************/
/*****************************************************************************/

// All tests of category 1,2,3 for FILES only

BOOST_AUTO_TEST_CASE(TailOfFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file tail display UC F1.DI2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 1Mb
    createFile<1000>(localFilePath);
    // Copy file on remote host
    BOOST_MESSAGE( "REMOTE PATH: " + fileFullPath1 );
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( tailOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of headOfFile function
    BOOST_CHECK( content.substr(content.size()-8,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(TailOfFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file tail display errors UC F1.DI2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    string content;
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( tailOfFile(sessionKey, invalidFullPath, content), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( tailOfFile(sessionKey, noAccessFullPath, content), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( tailOfFile(sessionKey, invalidMachineFullPath, content), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
