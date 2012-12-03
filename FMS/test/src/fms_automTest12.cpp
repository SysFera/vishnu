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

// namespaces declaration and  aliases

#include "FMS_test_utils.hpp"

using namespace std;
using namespace UMS_Data;
using namespace FMS_Data;
using namespace vishnu;
namespace ba = boost::assign;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

// The database, UMS and FMS SeD are launched by FMSSedFixture.
BOOST_FIXTURE_TEST_SUITE(SyncCopyFile, FMSSeDFixture)



// All tests of category 4

BOOST_AUTO_TEST_CASE(SyncCopyFile_Base)
{
  BOOST_TEST_MESSAGE("Testing synchronous copy of files UC F2.CP1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    touch<10>(localFilePath);

    // local to local (changing name)
//     BOOST_MESSAGE("Checking local to local copy");
//     string backupFileName = newFileName + ".bak";
//     string backupLocalPath = localDir + backupFileName;
//     BOOST_REQUIRE( cp(sessionKey, localFilePath, backupLocalPath) == 0);
    // Check
//     bool isLocalCopyFound = areFoundInDir(sessionKey, localDir, ba::list_of(backupFileName));
//     BOOST_CHECK(isLocalCopyFound);
    // Cleanup
//     BOOST_CHECK( rm(sessionKey, backupLocalPath) == 0);

    // local to remote
    BOOST_MESSAGE("Checking local to remote copy");
    BOOST_REQUIRE( cp(sessionKey, localFilePath, baseDirFullPath1) == 0);
    // Check
    bool isRemoteCopyFound = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(isRemoteCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

    // remote to local
    BOOST_MESSAGE("Checking remote to local copy");
    string localCopyName = newFileName + ".bak";
    string localCopyPath = localDir + slash + localCopyName;
    BOOST_CHECK( isFoundInDir(sessionKey, baseDirFullPath1, newFileName));
    BOOST_REQUIRE( cp(sessionKey, fileFullPath1, localCopyPath) == 0);
    // Check
    bool isLocalCopyFound = isFoundInLocalDir(localDir,localCopyName);
    BOOST_CHECK(isLocalCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

    // remote to remote
    BOOST_MESSAGE("Checking remote to remote copy");
    BOOST_REQUIRE( cp(sessionKey, fileFullPath1, baseDirFullPath2) == 0);
    // Check
    isRemoteCopyFound = isFoundInDir(sessionKey, baseDirFullPath2, newFileName);
    BOOST_CHECK(isRemoteCopyFound);
    // Cleanup
    BOOST_CHECK( rm(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( rm(sessionKey, fileFullPath2) == 0);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(SyncCopyFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing synchronous copy of files errors UC F2.CP1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    DirEntryList dirContent;
    touch<10>(localFilePath);
    // E1 case - wrong source path
    string invalidDir = "boifdo";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( cp(sessionKey, invalidFullPath, baseDirFullPath1), VishnuException);
    // E2 case - wrong destination path
    string invalidFullPath2 = baseDirFullPath1 + slash + invalidDir + slash;
    BOOST_CHECK_THROW( cp(sessionKey, localFilePath, invalidFullPath2), VishnuException);
    // E3 case - no access to source path
    string noAccessLocalPath = "/etc/ssh/ssh_host_dsa_key";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( cp(sessionKey, noAccessFullPath, baseDirFullPath1), VishnuException);
    // E3 case - no access to remote path
    string noAccessRemotePath = machineId1 + sep + "/root";
    BOOST_CHECK_THROW( cp(sessionKey, localFilePath, noAccessRemotePath), VishnuException);
    // E4 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( cp(sessionKey, invalidMachineFullPath, baseDirFullPath1), VishnuException);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
