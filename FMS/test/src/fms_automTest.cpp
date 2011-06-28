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
BOOST_GLOBAL_FIXTURE(FMSSeDFixture)

/*****************************************************************************/
/******************************* F I L E S ***********************************/
/*****************************************************************************/

// All tests of category 1,2,3 for FILES only
BOOST_AUTO_TEST_SUITE(singleMachineFile)

BOOST_AUTO_TEST_CASE(CreateFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file creation UC F1.CR1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createFile(sessionKey, fileFullPath1) == 0);

    // To check the success of createFile function
    FileStat fileStat;
    BOOST_CHECK( getFileInfo(sessionKey, fileFullPath1, fileStat) ==0  );

    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(CreateFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file creation errors UC F1.CR1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( createFile(sessionKey, invalidFullPath), VishnuException);
    // E2 case
    string noAccessDir = "/root";
    string noAccessFullPath = machineId1 + sep + noAccessDir;
    BOOST_CHECK_THROW( createFile(sessionKey, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( createFile(sessionKey, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(DeleteFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file deletion UC F1.DE1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createFile(sessionKey, fileFullPath1) == 0); // setup
    BOOST_REQUIRE( removeFile(sessionKey, fileFullPath1) == 0); // test
    FileStat fileStat;
    BOOST_CHECK_THROW( getFileInfo(sessionKey, fileFullPath1, fileStat), VishnuException );

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(DeleteFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file deletion errors UC F1.DE1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( removeFile(sessionKey, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( removeFile(sessionKey, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( removeFile(sessionKey, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_CASE(HeadOfFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file head display UC F1.DI1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 1Mb
    createFile<1000>(localFilePath);
    // Copy file on remote host
    BOOST_MESSAGE( "REMOTE PATH: " + fileFullPath1 );
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( headOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of headOfFile function
    BOOST_CHECK( content.substr(0,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(HeadOfFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file head display errors UC F1.DI1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    string content;
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( headOfFile(sessionKey, invalidFullPath, content), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( headOfFile(sessionKey, noAccessFullPath, content), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( headOfFile(sessionKey, invalidMachineFullPath, content), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

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

BOOST_AUTO_TEST_CASE(ContentOfFile_Base)
{
  BOOST_TEST_MESSAGE("Testing file content display UC F1.DI3-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 1Mb
    createFile<1000>(localFilePath);
    // Copy file on remote host
    BOOST_MESSAGE( "REMOTE PATH: " + fileFullPath1 );
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( contentOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of contentOfFile function
    BOOST_CHECK( content.substr(8,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
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

BOOST_AUTO_TEST_CASE(GetFileInfo_Base)
{

  BOOST_TEST_MESSAGE("Testing file info display UC F1.DI5-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 10Kb
    createFile<10>(localFilePath);
    // Copy file on remote host
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    FileStat stat;
    BOOST_REQUIRE( getFileInfo(sessionKey, fileFullPath1, stat) == 0);
    // To check the success
    BOOST_CHECK( stat.getSize() == 10240 );
    BOOST_CHECK( stat.getOwner() == userLogin);
    BOOST_CHECK( stat.getPerms() == 0644);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
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
    BOOST_CHECK_THROW( getFileInfo(sessionKey, invalidFullPath, stat), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( getFileInfo(sessionKey, noAccessFullPath, stat), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( getFileInfo(sessionKey, invalidMachineFullPath, stat), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ChangeFileRights_Base)
{
  BOOST_TEST_MESSAGE("Testing file rights change UC F1.CH1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 10Kb
    createFile<10>(localFilePath);
    // Copy file on remote host
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    BOOST_REQUIRE( chMod(sessionKey, 600, fileFullPath1) == 0);
    // To check the success
    FileStat stat;
    BOOST_REQUIRE( getFileInfo(sessionKey, fileFullPath1, stat) == 0);
    BOOST_CHECK( stat.getPerms() == 0600);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ChangeFileRights_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file rights change errors UC F1.CH1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( chMod(sessionKey, 600, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( chMod(sessionKey, 600, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( chMod(sessionKey, 600, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ChangeGroup_Base)
{
  BOOST_TEST_MESSAGE("Testing file group change UC F1.CH2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // Create a file 10Kb
    createFile<10>(localFilePath);
    // Copy file on remote host
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    BOOST_REQUIRE( chGrp(sessionKey, groupTest, fileFullPath1) == 0);
    // To check the success
    FileStat stat;
    BOOST_REQUIRE( getFileInfo(sessionKey, fileFullPath1, stat) == 0);
    BOOST_CHECK( stat.getGroup() == groupTest);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(ChangeGroup_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing file group change errors UC F1.CH2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newFileName;
    BOOST_CHECK_THROW( chGrp(sessionKey, groupTest, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( chGrp(sessionKey, groupTest, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( chGrp(sessionKey, groupTest, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()

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
 *
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
BOOST_AUTO_TEST_SUITE(singleMachineDir)

BOOST_AUTO_TEST_CASE(CreateDir_Base)
{
  BOOST_TEST_MESSAGE("Testing directory creation UC F1.CR2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createDir(sessionKey, dirFullPath1) == 0);
    // Check 1: list content of parent directory
    bool isNewDirFound = isFoundInDir(sessionKey, baseDirFullPath1, newDirName);
    BOOST_REQUIRE(isNewDirFound);
    // Check 2: create new file in new directory
    string fileFullPath = dirFullPath1 + slash + newFileName;
    BOOST_REQUIRE( createFile(sessionKey, fileFullPath) == 0 );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath) == 0);
    BOOST_CHECK( removeDir(sessionKey, dirFullPath1) == 0);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(CreateDir_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing directory creation errors UC F1.CR2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newDirName;
    BOOST_CHECK_THROW( createDir(sessionKey, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( createDir(sessionKey, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( createDir(sessionKey, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(DeleteDir_Base)
{
  BOOST_TEST_MESSAGE("Testing directory deletion UC F1.DE2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createDir(sessionKey, dirFullPath1) == 0); // setup
    BOOST_REQUIRE( removeDir(sessionKey, dirFullPath1) == 0); // test
    // Check: list content of parent directory
    bool isRemovedDirFound = isFoundInDir(sessionKey, baseDirFullPath1, newDirName);
    BOOST_REQUIRE(!isRemovedDirFound);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(DeleteDir_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing directory deletion errors UC F1.DE2-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( removeDir(sessionKey, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( removeDir(sessionKey, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( removeDir(sessionKey, invalidMachineFullPath), VishnuException);

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

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
    StringList dirContent;
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

/*****************************************************************************/
/************************** T R A N S F E R S ********************************/
/*****************************************************************************/


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
BOOST_AUTO_TEST_SUITE(fileAndDirTransfers)

BOOST_AUTO_TEST_CASE(SyncCopyFile_Base)
{
  BOOST_TEST_MESSAGE("Testing synchronous copy of files UC F2.CP1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    createFile<10>(localFilePath);

    // local to local (changing name)
//     BOOST_MESSAGE("Checking local to local copy");
//     string backupFileName = newFileName + ".bak";
//     string backupLocalPath = localDir + backupFileName;
//     BOOST_REQUIRE( copyFile(sessionKey, localFilePath, backupLocalPath) == 0);
    // Check
//     bool isLocalCopyFound = areFoundInDir(sessionKey, localDir, ba::list_of(backupFileName));
//     BOOST_CHECK(isLocalCopyFound);
    // Cleanup
//     BOOST_CHECK( removeFile(sessionKey, backupLocalPath) == 0);

    // local to remote
    BOOST_MESSAGE("Checking local to remote copy");
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, baseDirFullPath1) == 0);
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
    BOOST_REQUIRE( copyFile(sessionKey, fileFullPath1, localCopyPath) == 0);
    // Check
    bool isLocalCopyFound = isFoundInLocalDir(localDir,localCopyName);
    BOOST_CHECK(isLocalCopyFound);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());

    // remote to remote
    BOOST_MESSAGE("Checking remote to remote copy");
    BOOST_REQUIRE( copyFile(sessionKey, fileFullPath1, baseDirFullPath2) == 0);
    // Check
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

BOOST_AUTO_TEST_CASE(SyncCopyFile_Exceptions)
{
  BOOST_TEST_MESSAGE("Testing synchronous copy of files errors UC F2.CP1-E");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    StringList dirContent;
    createFile<10>(localFilePath);
    // E1 case - wrong source path
    string invalidDir = "boifdo";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir;
    BOOST_CHECK_THROW( copyFile(sessionKey, invalidFullPath, baseDirFullPath1), VishnuException);
    // E2 case - wrong destination path
    string invalidFullPath2 = baseDirFullPath1 + slash + invalidDir + slash;
    BOOST_CHECK_THROW( copyFile(sessionKey, localFilePath, invalidFullPath2), VishnuException);
    // E3 case - no access to source path
    string noAccessLocalPath = "/etc/ssh/ssh_host_dsa_key";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( copyFile(sessionKey, noAccessFullPath, baseDirFullPath1), VishnuException);
    // E3 case - no access to remote path
    string noAccessRemotePath = machineId1 + sep + "/root";
    BOOST_CHECK_THROW( copyFile(sessionKey, localFilePath, noAccessRemotePath), VishnuException);
    // E4 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir1;
    BOOST_CHECK_THROW( copyFile(sessionKey, invalidMachineFullPath, baseDirFullPath1), VishnuException);
    // Cleanup
    vishnu::deleteFile(localFilePath.c_str());
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(AsyncCopyFile_Base)
{
  BOOST_TEST_MESSAGE("Testing asynchronous copy of files UC F2.CP2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    createFile<1000>(localFilePath);

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
    StringList dirContent;
    FileTransfer transferInfo;
    createFile<10>(localFilePath);
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

BOOST_AUTO_TEST_CASE(AsyncMoveFile_Base)
{
  BOOST_TEST_MESSAGE("Testing asynchronous move of files UC F2.MV2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    createFile<10>(localFilePath);
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
    StringList dirContent;
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

BOOST_AUTO_TEST_CASE(CancelFileTransfer_Base)
{
  BOOST_TEST_MESSAGE("Testing file transfer cancel UC F2.CA1-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    createFile<100000>(localFilePath);
    // local to remote
    BOOST_MESSAGE("Checking local to remote cancel");
    BOOST_REQUIRE( copyAsyncFile(sessionKey, localFilePath, baseDirFullPath1, transferInfo) == 0);
    // Wait two seconds
    bpt::seconds sleepTime(2);
    boost::this_thread::sleep(sleepTime);
    // Check list
    FileTransferList transferList;
    LsTransferOptions transferOpts;
    BOOST_REQUIRE( listFileTransfers(sessionKey, transferList, transferOpts) == 0);
    bool transferFound = false;
    for (unsigned int i=0; i < transferList.getFileTransfers().size(); ++i) {
      if (transferList.getFileTransfers().get(i)->getTransferId() == transferInfo.getTransferId()) {
        transferFound = true;
        break;
      }
    }
    BOOST_CHECK_MESSAGE( transferFound, "The transfer was not found - id=" + transferInfo.getTransferId() );
    // Cancel
    StopTransferOptions stopOpts;
    stopOpts.setTransferId(transferInfo.getTransferId());
    BOOST_REQUIRE( stopFileTransfer(sessionKey, stopOpts) == 0 );
    // Check
    bool isRemoteCopyFound1 = isFoundInDir(sessionKey, baseDirFullPath1, newFileName);
    BOOST_CHECK(isRemoteCopyFound1);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    vishnu::deleteFile(localFilePath.c_str());

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
// THE END

