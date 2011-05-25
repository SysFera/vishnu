/**
 * \file fms_automTest
 * \brief Contains FMS API test implementation
 * References: VISHNU_D3_1b_FMS-PlanTests
 */
// Prerequisites for this test suite to work correctly:
// 1/ check the cmake parameters TEST_*
// 2/ setup a 'test' unix group on host TEST_FMS_HOST1
//    and add the user TEST_USER_LOGIN to this group

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
static const string remoteBaseDir = "/tmp";
static const string baseDirFullPath1 = machineId1 + sep + remoteBaseDir;
static const string baseDirFullPath2 = machineId2 + sep + remoteBaseDir;
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
    BOOST_CHECK( getFilesInfo(sessionKey, fileFullPath1, fileStat) ==0  );

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_CHECK_THROW( getFilesInfo(sessionKey, fileFullPath1, fileStat), VishnuException );

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( headOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of headOfFile function
    BOOST_CHECK( content.substr(0,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( tailOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of headOfFile function
    BOOST_CHECK( content.substr(content.size()-8,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_REQUIRE( copyFile(sessionKey, localFilePath, fileFullPath1) == 0);
    string content;
    BOOST_REQUIRE( contentOfFile(sessionKey, fileFullPath1, content) == 0);
    // To check the success of contentOfFile function
    BOOST_CHECK( content.substr(8,8) == "abcdefgh" );
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_REQUIRE( getFilesInfo(sessionKey, fileFullPath1, stat) == 0);
    // To check the success
    BOOST_CHECK( stat.getSize() == 10240 );
    BOOST_CHECK( stat.getOwner() == userLogin);
    BOOST_CHECK( stat.getPerms() == 0644);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    BOOST_CHECK_THROW( getFilesInfo(sessionKey, invalidFullPath, stat), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = machineId1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( getFilesInfo(sessionKey, noAccessFullPath, stat), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
    BOOST_CHECK_THROW( getFilesInfo(sessionKey, invalidMachineFullPath, stat), VishnuException);

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
    BOOST_REQUIRE( getFilesInfo(sessionKey, fileFullPath1, stat) == 0);
    BOOST_CHECK( stat.getPerms() == 0600);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
    BOOST_REQUIRE( getFilesInfo(sessionKey, fileFullPath1, stat) == 0);
    BOOST_CHECK( stat.getGroup() == groupTest);
    // Cleanup
    BOOST_CHECK( removeFile(sessionKey, fileFullPath1) == 0);
    BOOST_CHECK( removeFile(sessionKey, localFilePath) == 0);

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
    string invalidMachineFullPath = invalidMachineId + sep + remoteBaseDir;
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
 * \brief Check if given strings are found in directory contentOfFile
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
    BOOST_MESSAGE("@@@@ look for : " + *iterNames);
      bool isFound = false;
      vector<string>::const_iterator iter = dirContent.getStrings().begin();
      while (!isFound && !(iter == dirContent.getStrings().end())) {
        BOOST_MESSAGE("DIR CONTENT: " + *iter);
        isFound = (*iter).find(*iterNames) != string::npos;
        ++iter;
      }
      if (!isFound) {
        areFound = false;
        BOOST_MESSAGE("File " << *iterNames << " not found in dir " << dirFullPath);
      }
  }
  return areFound;
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
BOOST_AUTO_TEST_SUITE(singleMachineDir)

BOOST_AUTO_TEST_CASE(CreateDir_Base)
{
  BOOST_TEST_MESSAGE("Testing directory creation UC F1.CR2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createDir(sessionKey, dirFullPath1) == 0);
    // Check 1: list content of parent directory
    bool isNewDirFound = areFoundInDir(sessionKey, baseDirFullPath1, ba::list_of(newDirName));
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

BOOST_AUTO_TEST_CASE(DeleteDir_Base)
{
  BOOST_TEST_MESSAGE("Testing directory deletion UC F1.DE2-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    BOOST_REQUIRE( createDir(sessionKey, dirFullPath1) == 0); // setup
    BOOST_REQUIRE( removeDir(sessionKey, dirFullPath1) == 0); // test
    // Check: list content of parent directory
    bool isRemovedDirFound = areFoundInDir(sessionKey, baseDirFullPath1, ba::list_of(newDirName));
    BOOST_REQUIRE(!isRemovedDirFound);

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
    const vector<string> fileSuffixes = ba::list_of("1.2.3")("éàè")("$")("!:");
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


BOOST_AUTO_TEST_SUITE_END()

// THE END

