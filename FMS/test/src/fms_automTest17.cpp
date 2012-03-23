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
#include "api_ums.hpp"
// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace FMS_Data;
using namespace vishnu;
namespace ba = boost::assign;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;
#define BIGINT 1125899906842624 // 1 petaoctet

/***************************************************************************/
int logFakeFileTransferIntoDatabase(const std::string& sessionKey, const FMS_Data::FileTransfer& transferInfo) {

std::string errorMsgCleaned = "no error";
int processId= -1;

 std::string numsession = "(SELECT numsessionid FROM vsession where sessionKey='" + sessionKey +"'" + ")";
  
std::string sqlInsert= "insert into filetransfer (vsession_numsessionid,userId,clientMachineId,sourceMachineId, "
    "destinationMachineId,sourceFilePath,destinationFilePath, transferid,status,fileSize,trCommand,processid,errorMsg,startTime)"
    "values ("+numsession+",'"+transferInfo.getUserId()+"','"+ transferInfo.getClientMachineId()+"','"+transferInfo.getSourceMachineId()+"','"+transferInfo.getDestinationMachineId()+"','"
    +transferInfo.getSourceFilePath()+"','"+transferInfo.getDestinationFilePath() +"','"+transferInfo.getTransferId()+"',"+convertToString(transferInfo.getStatus())+","
    +convertToString(transferInfo.getSize())+","+convertToString(transferInfo.getTrCommand())+","+convertToString(processId)+",'"+errorMsgCleaned+"'" +",CURRENT_TIMESTAMP)";



std::string filename="/tmp/sqlTest";
{
std::ofstream ofs (filename.c_str());
ofs << sqlInsert;
}
BOOST_REQUIRE_EQUAL (restore (filename),0);


return 0;
}



/**********************************************************************************/

bool operator== (const FMS_Data::FileTransfer& ltransferInfo,const FMS_Data::FileTransfer& rtransferInfo ){

  return ( (ltransferInfo.getTransferId() == rtransferInfo.getTransferId())
        && (ltransferInfo.getStatus() == rtransferInfo.getStatus())
        && (ltransferInfo.getSize() == rtransferInfo.getSize())
      );

}

// The database, UMS and FMS SeD are launched by FMSSedFixture.
BOOST_FIXTURE_TEST_SUITE(BigFileTransfer, FMSSeDFixture)


BOOST_AUTO_TEST_CASE(AddFakeFileTransfer)
{
  BOOST_TEST_MESSAGE("Testing Big file transfer ");
  VishnuConnection vc(userId, userPwd);
  string sessionKey=vc.getSessionKey();

  try {
    FileTransfer transferInfo;
    transferInfo.setTransferId(bfs::unique_path().string());
    transferInfo.setStatus(3);
    transferInfo.setUserId(userId);
    transferInfo.setClientMachineId("clientMachine");
    transferInfo.setSourceMachineId("sourceMachine");
    transferInfo.setDestinationMachineId("sourceMachine");
    transferInfo.setSize(BIGINT);
    transferInfo.setTrCommand(0);
    transferInfo.setSourceFilePath("path/to/src");
    transferInfo.setDestinationFilePath("path/to/dest");
    BOOST_REQUIRE_EQUAL(logFakeFileTransferIntoDatabase(sessionKey,  transferInfo),0);

    LsTransferOptions   options;
    FileTransferList    fileTransferList;
    options.setTransferId(transferInfo.getTransferId());
    BOOST_REQUIRE_EQUAL(listFileTransfers(sessionKey, fileTransferList, options) , 0); 
    BOOST_REQUIRE_EQUAL(fileTransferList.getFileTransfers().size(), 1); 
      FileTransfer* ft = fileTransferList.getFileTransfers().get(0);
      BOOST_CHECK(transferInfo == *ft);
  }catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}
BOOST_AUTO_TEST_SUITE_END()
  // THE END
