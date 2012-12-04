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
BOOST_FIXTURE_TEST_SUITE(GetFileInfo, FMSSeDFixture)

/*****************************************************************************/
/******************************* F I L E S ***********************************/
/*****************************************************************************/

// All tests of category 1,2,3 for FILES only

BOOST_AUTO_TEST_CASE(GetFileInfo_Base)
{

  BOOST_TEST_MESSAGE("Testing file info display UC F1.DI5-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 10Kb
    touch<10>(localFilePath);
    // Copy file on remote host
    BOOST_REQUIRE( cp(sessionKey, localFilePath, fileFullPath1) == 0);
    FileStat stat;
    BOOST_REQUIRE( stat(sessionKey, fileFullPath1, stat) == 0);
    // To check the success
    BOOST_CHECK( stat.getSize() == 10240 );
    BOOST_CHECK( stat.getOwner() == userLogin);
    BOOST_CHECK( stat.getPerms() == 0644);
    // Cleanup
    BOOST_CHECK( rm(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(GetFileInfo_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file info display errors UC F1.DI5-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileStat stat;
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( stat(sessionKey, invalidFullPath, stat), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( stat(sessionKey, noAccessFullPath, stat), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( stat(sessionKey, invalidMachineFullPath, stat), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
