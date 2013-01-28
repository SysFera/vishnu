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
BOOST_FIXTURE_TEST_SUITE(CreateDir, FMSSeDFixture)

/*****************************************************************************/
/************************  D I R E C T O R I E S *****************************/
/*****************************************************************************/

// All tests of category 1,2,3 for DIRECTORIES only

BOOST_AUTO_TEST_CASE(CreateDir_Base)
{
  std::string newFileName = "Test_FMS_File";
  std::string newDirName = "Test_FMS_Dir";
  std::string newSubDirName = "Test_FMS_Sub_Dir";
  std::string baseDirFullPath1 = m_test_fms_host1 + ":" + m_test_fms_dir1;
  std::string baseDirFullPath2 = m_test_fms_host1 + ":" + m_test_fms_dir2;
  std::string fileFullPath1 = baseDirFullPath1 + "/" + newFileName;
  std::string fileFullPath2 = baseDirFullPath2 + "/" + newFileName;
  std::string dirFullPath1 = baseDirFullPath1 + "/" + newDirName;
  std::string recursiveDirFullPath1 = dirFullPath1 + "/" +  newSubDirName;
  std::string dirFullPath2 = baseDirFullPath2 + "/" + newDirName;
  std::string localFilePath = m_test_fms_working_dir + "/" + newFileName;
  
  BOOST_TEST_MESSAGE("Testing directory creation UC F1.CR2-B");
  VishnuConnection vc(m_test_fms_user_login, m_test_fms_user_pwd);
  string sessionKey=vc.getSessionKey();
  string slash = "/";
  string sep = ":";

  try {
     if(isFoundInDir(sessionKey,baseDirFullPath1,newDirName)){
      RmFileOptions rmOptions;
      rmOptions.setIsRecursive (true);
      BOOST_REQUIRE( rm(sessionKey, dirFullPath1,rmOptions) == 0);

    }
    BOOST_REQUIRE( mkdir(sessionKey, dirFullPath1) == 0);
    // Check 1: list content of parent directory
    bool isNewDirFound = isFoundInDir(sessionKey, baseDirFullPath1, newDirName);
    BOOST_REQUIRE(isNewDirFound);
    // Check 2: create new file in new directory
    string fileFullPath = dirFullPath1 + slash + newFileName;
    BOOST_REQUIRE( touch(sessionKey, fileFullPath) == 0 );
    // Cleanup
    BOOST_REQUIRE( rm(sessionKey, fileFullPath) == 0);
    
    // check 3 :recursive directory creation 
    CreateDirOptions mkdirOptions;
    mkdirOptions.setIsRecursive (true);
    BOOST_CHECK_EQUAL( mkdir(sessionKey, recursiveDirFullPath1,mkdirOptions),0); 
    // Check 4: list content of parent directory 
    isNewDirFound = isFoundInDir(sessionKey,dirFullPath1 , newSubDirName);
    BOOST_REQUIRE(isNewDirFound);
    RmFileOptions rmOptions;
    rmOptions.setIsRecursive (true);
    BOOST_REQUIRE ( rm(sessionKey,dirFullPath1 ,rmOptions) == 0);
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_CASE(CreateDir_Exceptions)
{
  std::string newFileName = "Test_FMS_File";
  std::string newDirName = "Test_FMS_Dir";
  std::string newSubDirName = "Test_FMS_Sub_Dir";
  std::string baseDirFullPath1 = m_test_fms_host1 + ":" + m_test_fms_dir1;
  std::string baseDirFullPath2 = m_test_fms_host1 + ":" + m_test_fms_dir2;
  std::string fileFullPath1 = baseDirFullPath1 + "/" + newFileName;
  std::string fileFullPath2 = baseDirFullPath2 + "/" + newFileName;
  std::string dirFullPath1 = baseDirFullPath1 + "/" + newDirName;
  std::string recursiveDirFullPath1 = dirFullPath1 + "/" +  newSubDirName;
  std::string dirFullPath2 = baseDirFullPath2 + "/" + newDirName;
  std::string localFilePath = m_test_fms_working_dir + "/" + newFileName;
  
  BOOST_TEST_MESSAGE("Testing directory creation errors UC F1.CR2-E");
  VishnuConnection vc(m_test_fms_user_login, m_test_fms_user_pwd);
  string sessionKey=vc.getSessionKey();
  string slash = "/";
  string sep = ":";

  try {
    // E1 case
    string invalidDir = "rkvh";
    string invalidFullPath = baseDirFullPath1 + slash + invalidDir + slash + newDirName;
    BOOST_CHECK_THROW( mkdir(sessionKey, invalidFullPath), VishnuException);
    // E2 case
    string noAccessLocalPath = "/root/abc";
    string noAccessFullPath = m_test_fms_host1 + sep + noAccessLocalPath;
    BOOST_CHECK_THROW( mkdir(sessionKey, noAccessFullPath), VishnuException);
    // E3 case
    string invalidMachineId = "tt";
    string invalidMachineFullPath = invalidMachineId + sep + m_test_fms_dir1;
    BOOST_CHECK_THROW( mkdir(sessionKey, invalidMachineFullPath), VishnuException);

// E4 case: recursive directories creation
// assert dirFullPath1 is not in baseDirFullPath1 
    bool isNewDirFound = isFoundInDir(sessionKey, baseDirFullPath1,dirFullPath1);
    BOOST_REQUIRE(!isNewDirFound);
BOOST_CHECK_THROW( mkdir(sessionKey, recursiveDirFullPath1),VishnuException ); 


  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
// THE END
