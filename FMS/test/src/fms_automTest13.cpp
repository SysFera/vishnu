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

//Vishnu Headers
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
BOOST_FIXTURE_TEST_SUITE(AsyncCopyFile, FMSSeDFixture)


// All tests of category 4

BOOST_AUTO_TEST_CASE(AsyncCopyFile_Base)
{
  BOOST_TEST_MESSAGE("Testing asynchronous copy of files UC F2.CP2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    touch<1000>(localFilePath);

    // local to remote
    BOOST_MESSAGE("Checking local to remote copy");
    BOOST_REQUIRE( copyAsyncFile(sessionKey, localFilePath, baseDirFullPath1, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    bool isRemoteCopyFound = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(isRemoteCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

    // remote to local
    BOOST_MESSAGE("Checking remote to local copy");
    string localCopyName = newFileName + ".bak";
    string localCopyPath = localDir + slash + localCopyName;
    BOOST_REQUIRE( copyAsyncFile(sessionKey, fileFullPath1, localCopyPath, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    bool isLocalCopyFound = isFoundInLocalDir(localDir, localCopyName);
    BOOST_CHECK(isLocalCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

    // remote to remote
    BOOST_MESSAGE("Checking remote to remote copy");
    BOOST_REQUIRE( copyAsyncFile(sessionKey, fileFullPath1, baseDirFullPath2, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    isRemoteCopyFound = isFoundInDir(sessionKey, baseDirFullPath2, newFileName);
    BOOST_CHECK(isRemoteCopyFound);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, fileFullPath2) == 0);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(AsyncCopyFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing asynchronous copy of files errors UC F2.CP2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    DirEntryList dirContent;
    FileTransfer transferInfo;
    touch<10>(localFilePath);
    // E1 case - wrong source path
    BOOST_MESSAGE("Check wrong source path");
    string invalidDir = "erli";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( copyAsyncFile(sessionKey, invalidFullPath, baseDirFullPath1, transferInfo), VishnuException);
    // E2 case - wrong destination path
    BOOST_MESSAGE("Check wrong destination path");
    string invalidFullPath2 = baseDirFullPath1 + slash + invalidDir + slash;
    BOOST_REQUIRE( copyAsyncFile(sessionKey, localFilePath, invalidFullPath2, transferInfo) == 0);
    BOOST_MESSAGE("Launched transfer: " + transferInfo.getTransferId());
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E3 case - no access to source path
    BOOST_MESSAGE("Check unaccessible source path");
    string noAccessLocalPath = "/etc/ssh/ssh_host_dsa_key";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_REQUIRE( copyAsyncFile(sessionKey, noAccessFullPath, baseDirFullPath1, transferInfo) == 0);
    BOOST_MESSAGE("Launched transfer: " + transferInfo.getTransferId());
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E3 case - no access to remote path
    BOOST_MESSAGE("Check unaccessible destination path");
    string noAccessRemotePath = machineId1 + sep + "/root";
    BOOST_REQUIRE( copyAsyncFile(sessionKey, localFilePath, noAccessRemotePath, transferInfo) == 0);
    BOOST_MESSAGE("Launched transfer: " + transferInfo.getTransferId());
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E4 case
    BOOST_MESSAGE("Check invalid machine");
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( copyAsyncFile(sessionKey, invalidMachineFullPath, baseDirFullPath1, transferInfo), VishnuException);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
