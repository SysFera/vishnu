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

// Vishnu Headers

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
BOOST_FIXTURE_TEST_SUITE(ListDirContent, FMSSeDFixture)

/*****************************************************************************/
/************************  D I R E C T O R I E S *****************************/
/*****************************************************************************/


// All tests of category 1,2,3 for DIRECTORIES only

BOOST_AUTO_TEST_CASE(ListDirContent_Base)
{
  BOOST_TEST_MESSAGE("Testing directory list content UC F1.DI4-B");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    
    if(isFoundInDir(sessionKey,baseDirFullPath1,newDirName)){
      RmFileOptions rmOptions;
      rmOptions.setIsRecursive (true);
      BOOST_REQUIRE( removeFile(sessionKey, dirFullPath1,rmOptions) == 0);

    }
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
