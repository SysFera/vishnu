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

#include "FMS_test_utils.hpp"
// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace FMS_Data;
using namespace vishnu;
namespace ba = boost::assign;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

// The database, UMS and FMS SeD are launched by FMSSedFixture.
BOOST_FIXTURE_TEST_SUITE(ContentOfFile, FMSSeDFixture)

/*****************************************************************************/
/******************************* F I L E S ***********************************/
/*****************************************************************************/

// All tests of category 1,2,3 for FILES only

BOOST_AUTO_TEST_CASE(ContentOfFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file content display UC F1.DI3-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 1Mb
    touch<1000>(localFilePath);
    // Copy file on remote host
    BOOST_MESSAGE( "REMOTE PATH: " + fileFullPath1 );
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( contentOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of contentOfFile function
    BOOST_CHECK( content.substr(8,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( rm(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ContentOfFile_Exceptions)
{

  BOOST_TEST_MESSAGE("Testing file content display errors UC F1.DI3-E");

  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    string content;
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( contentOfFile(sessionKey, invalidFullPath, content), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( contentOfFile(sessionKey, noAccessFullPath, content), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( contentOfFile(sessionKey, invalidMachineFullPath, content), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
