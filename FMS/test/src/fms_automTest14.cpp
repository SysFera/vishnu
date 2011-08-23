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
BOOST_FIXTURE_TEST_SUITE(SyncMoveFile, FMSSeDFixture)

/*****************************************************************************/
/************************** T R A N S F E R S ********************************/
/*****************************************************************************/

/**
 * \brief Check if given strings are found in directory
 * \param sessionKey  the session key
 * \param dirFullPath the directory complete path (host:path)
 * \param names       the strings to search for (vector)
 * \return true if all names are found within the directory long content
 */
bool areFoundInDir(const string& sessionKey,
                   const string& dirFullPath,
                   const vector<string>& names) {
  StringList dirContent;
  LsDirOptions lsOptions;
  lsOptions.setLongFormat(true);
  BOOST_REQUIRE( listDir(sessionKey, dirFullPath, dirContent) ==0  );
  bool areFound = true;
  for (vector<string>::const_iterator iterNames = names.begin();
    iterNames != names.end();
    ++iterNames) {
      bool isFound = false;
      vector<string>::const_iterator iter = dirContent.getStrings().begin();
      while (!isFound && !(iter == dirContent.getStrings().end())) {
        isFound = (*iter).find(*iterNames) != string::npos;
        ++iter;
      }
      if (!isFound) {
        areFound = false;
      }
  }
  return areFound;
}

/**
 * \brief Check if a given string is found in directory
 * \param sessionKey  the session key
 * \param dirFullPath the directory complete path (host:path)
 * \param name        the string to search for
 * \return true name is found within the directory long content
 */
bool isFoundInDir(const string& sessionKey,
                  const string& dirFullPath,
                  const string& name) {
  return areFoundInDir(sessionKey, dirFullPath, ba::list_of(name));
}
/**
 * \brief Check if a given string is found in a local directory (must match a FILE name)
 * \param dirLocalPath the directory local path
 * \param name        the string to search for
 * \return true name is found within the directory long content
 */
bool isFoundInLocalDir(const string& dirLocalPath,
                       const string& name) {
  bfs::path dir(dirLocalPath);
  bfs::directory_iterator end_iter;
  if ( bfs::exists(dir) && bfs::is_directory(dir)) {
    for( bfs::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter) {
      if ( bfs::is_regular_file(dir_iter->status())
            && dir_iter->path().leaf() == name) {
        return true;
      }
    }
  }
  return false;
}


/**
 * \brief Wait until termination of an asynchronous file transfer
 * \param sessionKey the session Key
 * \param transferInfo  the file transfer information
 * \return the final status value of the file transfer (-1 if error)
 */
int
waitAsyncCopy(const string& sessionKey, const FileTransfer& transferInfo) {
  unsigned int        pollCounter = 10;
  bool                terminated = false;
  LsTransferOptions   options;
  FileTransferList    fileTransferList;
  options.setTransferId(transferInfo.getTransferId());
  while (!terminated && pollCounter--) {
    if (listFileTransfers(sessionKey, fileTransferList, options) != 0) {
      BOOST_MESSAGE("ERROR: Could not retrieve file transfer information");
      return -1;
    }
    if (fileTransferList.getFileTransfers().size() == 1) {
      FileTransfer* ft = fileTransferList.getFileTransfers().get(0);
      if (ft->getStatus() != STATUS_INPROGRESS) {
        BOOST_MESSAGE("Async transfer is terminated! - status = " + convertToString(ft->getStatus()));
        if (ft->getStatus() == STATUS_FAILED) {
          BOOST_MESSAGE("Transfer failed message: " + ft->getErrorMsg());
        }
        terminated = true;
      }
    } else if (fileTransferList.getFileTransfers().size() == 0) {
      BOOST_MESSAGE("ERROR: File transfer list is empty for a given transferId!");
      return -1;
    } else {
      BOOST_MESSAGE("ERROR: File transfer list contains more than 1 item for a given transferId!");
      return -1;
    }
    bpt::seconds sleepTime(5);
    boost::this_thread::sleep(sleepTime);
  }
  if (!terminated) {
    BOOST_MESSAGE("ERROR: End of polling for file transfer");
    return -1;
  }
  return fileTransferList.getFileTransfers().get(0)->getStatus();
}

// All tests of category 4

BOOST_AUTO_TEST_CASE(SyncMoveFile_Base)
{
  BOOST_TEST_MESSAGE("Testing synchronous move of files UC F2.MV1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    createFile<10>(localFilePath);
    // local to remote
    BOOST_MESSAGE("Checking local to remote move");
    BOOST_MESSAGE("SRC=" + localFilePath);
    BOOST_MESSAGE("DST=" + baseDirFullPath1);
    BOOST_REQUIRE( moveFile(sessionKey, localFilePath, baseDirFullPath1) == 0);
    // Check
    bool isLocalSourceFound = isFoundInLocalDir(localDir, newFileName);
    BOOST_CHECK(!isLocalSourceFound);
    bool isRemoteCopyFound1 = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(isRemoteCopyFound1);

    // remote to remote (using file from previous step)
    BOOST_MESSAGE("Checking remote to remote move");
    BOOST_REQUIRE( moveFile(sessionKey, fileFullPath1, baseDirFullPath2) == 0);
    // Check
    isRemoteCopyFound1 = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(!isRemoteCopyFound1);
    bool isRemoteCopyFound2 = isFoundInDir(sessionKey, baseDirFullPath2, newFileName);
    BOOST_CHECK(isRemoteCopyFound2);

    // remote to local (using file from previous step)
    BOOST_MESSAGE("Checking remote to local move");
    BOOST_REQUIRE( moveFile(sessionKey, fileFullPath2, localDir) == 0);
    // Check
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

BOOST_AUTO_TEST_CASE(SyncMoveFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing synchronous move of files errors UC F2.MV1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    StringList dirContent;
    createFile<10>(localFilePath);
    // E1 case - wrong source path
    BOOST_MESSAGE("Check wrong source path");
    string invalidDir = "rtiui";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( moveFile(sessionKey, invalidFullPath, baseDirFullPath1), VishnuException);
    // E2 case - wrong destination path
    BOOST_MESSAGE("Check wrong destination path");
    string invalidFullPath2 = baseDirFullPath1 + slash + invalidDir + slash;
    BOOST_CHECK_THROW( moveFile(sessionKey, localFilePath, invalidFullPath2), VishnuException);
    // E3 case - no access to source path
    BOOST_MESSAGE("Check unaccessible source path");
    string noAccessLocalPath = "/etc/ssh/ssh_host_dsa_key";;
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( moveFile(sessionKey, noAccessFullPath, baseDirFullPath1), VishnuException);
    // E3 case - no access to remote path
    BOOST_MESSAGE("Check unaccessible destination path");
    string noAccessRemotePath = machineId1 + sep + "/root";
    BOOST_CHECK_THROW( moveFile(sessionKey, localFilePath, noAccessRemotePath), VishnuException);
    // E4 case
    BOOST_MESSAGE("Check invalid machine");
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( moveFile(sessionKey, invalidMachineFullPath, baseDirFullPath1), VishnuException);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
