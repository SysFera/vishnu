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
BOOST_FIXTURE_TEST_SUITE(AsyncMoveFile, FMSSeDFixture)


// All tests of category 4

BOOST_AUTO_TEST_CASE(AsyncMoveFile_Base)
{
  BOOST_TEST_MESSAGE("Testing asynchronous move of files UC F2.MV2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    createFile<1000>(localFilePath);
    // local to remote
    BOOST_MESSAGE("Checking local to remote move");
    BOOST_REQUIRE( moveAsyncFile(sessionKey, localFilePath, baseDirFullPath1, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    bool isLocalSourceFound = isFoundInLocalDir(localDir, newFileName);
    BOOST_CHECK(!isLocalSourceFound);
    bool isRemoteCopyFound1 = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(isRemoteCopyFound1);

    // remote to remote (using file from previous step)
    BOOST_MESSAGE("Checking remote to remote move");
    BOOST_REQUIRE( moveAsyncFile(sessionKey, fileFullPath1, baseDirFullPath2, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    isRemoteCopyFound1 = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(!isRemoteCopyFound1);
    bool isRemoteCopyFound2 = isFoundInDir(sessionKey, baseDirFullPath2, newFileName);
    BOOST_CHECK(isRemoteCopyFound2);

    // remote to local (using file from previous step)
    BOOST_MESSAGE("Checking remote to local move");
    BOOST_REQUIRE( moveAsyncFile(sessionKey, fileFullPath2, localDir, transferInfo) == 0);
    // Check
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_COMPLETED );
    isRemoteCopyFound2 = isFoundInDir(sessionKey, baseDirFullPath2, newFileName);
    BOOST_CHECK(!isRemoteCopyFound2);
    bool isLocalCopyFound = isFoundInLocalDir(localDir, newFileName);
    BOOST_CHECK(isLocalCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(AsyncMoveFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing asynchronous move of files errors UC F2.MV2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    DirEntryList dirContent;
    FileTransfer transferInfo;
    createFile<10>(localFilePath);
    // E1 case - wrong source path
    BOOST_MESSAGE("Check wrong source path");
    string invalidDir = "dffoize";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( moveAsyncFile(sessionKey, invalidFullPath, baseDirFullPath1, transferInfo), VishnuException);
    // E2 case - wrong destination path
    BOOST_MESSAGE("Check wrong destination path");
    string invalidFullPath2 = baseDirFullPath1 + slash + invalidDir + slash;
    BOOST_REQUIRE( moveAsyncFile(sessionKey, localFilePath, invalidFullPath2, transferInfo) == 0);
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E3 case - no access to source path
    BOOST_MESSAGE("Check unaccessible source path");
    string noAccessLocalPath = "/etc/ssh/ssh_host_dsa_key";;
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_REQUIRE( moveAsyncFile(sessionKey, noAccessFullPath, baseDirFullPath1, transferInfo) == 0);
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E3 case - no access to remote path
    BOOST_MESSAGE("Check unaccessible destination path");
    string noAccessRemotePath = machineId1 + sep + "/root";
    BOOST_REQUIRE( moveAsyncFile(sessionKey, localFilePath, noAccessRemotePath, transferInfo) == 0);
    BOOST_REQUIRE( waitAsyncCopy(sessionKey, transferInfo) == STATUS_FAILED );
    // E4 case
    BOOST_MESSAGE("Check invalid machine");
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( moveAsyncFile(sessionKey, invalidMachineFullPath, baseDirFullPath1, transferInfo), VishnuException);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
