/**
 * \file ims_automTest
 * \brief Contains IMS API test implementation
 * References: VISHNU_D5_1b_IMS-PlanTests
 */
/*********************************************************
#####Tests prerequisites:#######
- All ums and tms prerequisites must be considered
- The Cmake variable TEST_LOCAL_HOSTNAME, which is the name of the
local machine on which the tests will be launched,
must be defined.

- The Cmake variable TEST_ROOT_LOGIN which is the name of the user unix
account of local machine must be defined

*********************************************************/

//UMS forward Headers
#include "UMS_Data_forward.hpp"
//IMS forward Headers
#include "IMS_Data_forward.hpp"
#include "IMS_fixtures.hpp"
#include "vishnuTestUtils.hpp"
#include "utilVishnu.hpp"
#include "IMS_testconfig.h"

#include "TMS_Data.hpp"
using namespace TMS_Data;

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace IMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

static const string adminId = "root";
static const string adminPwd = "vishnu_user";
static const string userId = "user_1";
static const string userPwd = "toto";
static const string sqlPath = IMSSQLPATH;
static const string machineId="machine_1";
static const string badMachineId="unknown_name";
static const string sshCmd =" ssh -o PasswordAuthentication=no ";

BOOST_FIXTURE_TEST_SUITE(export_command, IMSSeDFixture)

//Test category 1
//I3-B: export and replay commands
BOOST_AUTO_TEST_CASE(export_command_normal_call) 
{

  BOOST_TEST_MESSAGE("Use case I3 – B: Export and replay commands normal call");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  // Command history
  UMS_Data::ListCommands listCmd;
  UMS_Data::ListCmdOptions listCmdOpt ;
  // List Sessions
  ListSessions listSess;
  ListSessionOptions listSessionsOpt;
  //Options for export
  IMS_Data::ExportOp exportOp;
  //1 for Shell export
  exportOp.setExportType(1);
  string filename = "tmpTest.txt";
  string filePath;
  string fileContent;
  string tmp;
  //Session in which the commands will be exported
  Session sess ;
  ConnectOptions connectOpt;
  //Set close policy : on disconnect
  connectOpt.setClosePolicy(2);
  //ListJobs
  ListJobs lsJobs;
  ListJobsOptions lsOptions;
  //To list metric history
  IMS_Data::ListMetric list;
  IMS_Data::MetricHistOp op;
  //Set FREEMOMORY metric
  op.setType(5);

  try {
    //To open a session to launch commands
    BOOST_CHECK_EQUAL(connect(adminId, adminPwd, sess, connectOpt), 0);
    //To list sessions
    BOOST_CHECK_EQUAL(listSessions(sess.getSessionKey(), listSess, listSessionsOpt), 0);
    //To list jobs
    BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId, lsJobs, lsOptions), 0);
    //To list metric history
    BOOST_CHECK_EQUAL(getMetricHistory(sess.getSessionKey(), machineId, list, op),0  );
    listCmdOpt.setSessionId(sess.getSessionId());
    //To list the commands
    BOOST_CHECK_EQUAL(vishnu::listHistoryCmd(sess.getSessionKey(), listCmd, listCmdOpt), 0);
    BOOST_REQUIRE(listCmd.getCommands().size() != 0);
    //To close session
    BOOST_CHECK_EQUAL(close (sess.getSessionKey()), 0);
    //To create a locate file for the test
    std::ofstream file(filename.c_str());
    filePath = boost::filesystem3::current_path().string() +"/"+ filename;
    //To create the file in which the command will be saved
    if (!file.is_open()) {
      throw UserException(ERRCODE_INVALID_PARAM, "Error opening file: " + filename);
    }
    BOOST_CHECK_EQUAL(exportCommands(sessionKey, sess.getSessionId(), filePath, exportOp), 0);
    fileContent = vishnu::get_file_content(filePath.c_str());
    //To check if the file is not empty
    BOOST_REQUIRE(fileContent.size() != 0);
    //To check the mapping between export and listHistoryCmd
    for(unsigned int i = 0; i < listCmd.getCommands().size(); i++) {
      tmp = listCmd.getCommands().get(i)->getCmdDescription();
      //To check if the file contains the current command
      BOOST_REQUIRE(fileContent.find(tmp)!=string::npos);
    }
    //To remove the temporary file
    vishnu::deleteFile(filePath.c_str());
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
    try {
      //To remove the temporary file
      vishnu::deleteFile(filePath.c_str());
    } catch (exception& exp) {
      BOOST_MESSAGE(exp.what());
    }
  }
}

//I3-E1: export and replay commands with bad old session Id
BOOST_AUTO_TEST_CASE(export_command_bad_old_session_Id_call) {

  BOOST_TEST_MESSAGE("Use case I3 – E1: Export and replay commands with bad old session Id call");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //Options for export
  IMS_Data::ExportOp exportOp;
  //1 for Shell export
  exportOp.setExportType(1);
  string filename = "tmpTest.txt";
  string filePath;
  string badSessionId = "badSessionId";
  //To create a locate file for the test
  std::ofstream file(filename.c_str());
  filePath = boost::filesystem3::current_path().string() +"/"+ filename;
  //To create the file in which the command will be saved
  if (!file.is_open()) {
    throw UserException(ERRCODE_INVALID_PARAM, "Error opening file: " + filename);
  }
  BOOST_CHECK_THROW(exportCommands(sessionKey, badSessionId, filePath, exportOp), VishnuException);
  //To remove the temporary file
  vishnu::deleteFile(filePath.c_str());
}

//I3-E2: export and replay commands with bad file path
BOOST_AUTO_TEST_CASE(export_command_bad_file_path_call) {

  BOOST_TEST_MESSAGE("Use case I3 – E2: Export and replay commands with bad file path");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //Options for export
  IMS_Data::ExportOp exportOp;
  //1 for Shell export
  exportOp.setExportType(1);
  string filePath = "/home/Paco/DeLaStagna";
  //Session
  Session sess ;
  //Connection options
  ConnectOptions connectOpt;
  //Set close policy : on disconnect
  connectOpt.setClosePolicy(2);
  //To open a session
  BOOST_CHECK_EQUAL(connect(adminId, adminPwd, sess, connectOpt), 0);
  //To close session
  BOOST_CHECK_EQUAL(close (sess.getSessionKey()), 0);

  BOOST_CHECK_THROW(exportCommands(sessionKey, sess.getSessionId(), filePath, exportOp), VishnuException);

}

BOOST_AUTO_TEST_SUITE_END()
// THE END

