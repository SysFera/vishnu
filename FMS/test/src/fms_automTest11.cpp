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
BOOST_FIXTURE_TEST_SUITE(ListDirContent, FMSSeDFixture)

/*****************************************************************************/
/************************  D I R E C T O R I E S *****************************/
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
  DirEntryList dirContent;
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
 * \brief Add prefix to a vector of strings
 * \param prefix  the common prefix
 * \param suffixes  the vector of strings
 * \return the modified vector of strings (prefix + suffixes[i])
 */
vector<string>&
addPrefix(const string& prefix, vector<string>& suffixes) {
  for (vector<string>::iterator iter = suffixes.begin();
    iter != suffixes.end();
    ++iter  )
    {
      (*iter) = prefix + (*iter);
   }
  return suffixes;
}
/**
 * \brief Create several files on remote machine
 * \param sessionKey  the session key
 * \param localFilePaths  the local paths for the files
 * \param remoteFullPath  the destination full path (directory)
 */
void
createRemoteFiles(const string& sessionKey,
                  const vector<string>& localFilePaths,
                  const string& remoteFullPath) {
  for (vector<string>::const_iterator iter = localFilePaths.begin();
    iter != localFilePaths.end();
    ++iter) {
    const string& localFilePath = *iter;
    createFile<1>(localFilePath);
    BOOST_MESSAGE("CreateRemoteFile: " << localFilePath);
    string errorMsg =  "Could not copy file ";
    errorMsg.append(localFilePath);
    errorMsg.append(" on remote host");
    BOOST_REQUIRE_MESSAGE( copyFile(sessionKey, localFilePath, remoteFullPath) == 0,
      errorMsg);
  }
}

// All tests of category 1,2,3 for DIRECTORIES only

BOOST_AUTO_TEST_CASE(ListDirContent_Base)
{
  BOOST_TEST_MESSAGE("Testing directory list content UC F1.DI4-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createDir(sessionKey, dirFullPath1) == 0);
    // put some content into the directory
    const vector<string> fileSuffixes = ba::list_of("1.2.3")("éàè")("$")("!~-_#");
    vector<string> localFiles = fileSuffixes;
    createRemoteFiles(sessionKey, addPrefix(localFilePath, localFiles), dirFullPath1);

    vector<string> remoteFileNames = fileSuffixes;
    bool areFilesFound = areFoundInDir(sessionKey, dirFullPath1, addPrefix(newFileName, remoteFileNames));
    BOOST_REQUIRE(areFilesFound);

    // Cleanup
    vector<string> remoteFullPaths = fileSuffixes;
    addPrefix(dirFullPath1 + slash + newFileName, remoteFullPaths);
    for (vector<string>::const_iterator iter = remoteFullPaths.begin();
      iter != remoteFullPaths.end();
      ++iter) {
        BOOST_CHECK_MESSAGE( removeFile(sessionKey, *iter) == 0,
          "Could not remove file " + *iter);
    }
    BOOST_CHECK( removeDir(sessionKey, dirFullPath1) == 0);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ListDirContent_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing directory deletion errors UC F1.DE2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    DirEntryList dirContent;
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( listDir(sessionKey, invalidFullPath, dirContent), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( listDir(sessionKey, noAccessFullPath, dirContent), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( listDir(sessionKey, invalidMachineFullPath, dirContent), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
