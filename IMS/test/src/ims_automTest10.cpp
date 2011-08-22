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

BOOST_FIXTURE_TEST_SUITE(get_running_processes, IMSSeDFixture)

//Test category 3
//IA1-B:  Get the running processes normal call
// Get the running processes: normal call
BOOST_AUTO_TEST_CASE(get_running_processes_normal_call) 
{

  BOOST_TEST_MESSAGE("Use case IA1-B: Get the running processes normal call");
  bool imssedFound = false;
  bool tmssedFound = false;
  bool umssedFound = false;
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();

  IMS_Data::ListProcesses listProcess;
  IMS_Data::ProcessOp op;
  op.setMachineId(machineId);

  try {
    BOOST_CHECK_EQUAL(getProcesses(sessionKey, listProcess, op),0  );
    BOOST_REQUIRE(listProcess.getProcess().size() != 0);

    //To check if the main processes are on the list of processes
    for(unsigned int i = 0; i < listProcess.getProcess().size(); i++) {
      //To check imssed process
      if (listProcess.getProcess().get(i)->getProcessName().compare("IMS") == 0) {
        imssedFound = true;
      }
      //To check tmssed process
      if (listProcess.getProcess().get(i)->getProcessName().compare("TMS") == 0) {
        tmssedFound = true;
      }
      //To check umssed process
      if (listProcess.getProcess().get(i)->getProcessName().compare("UMS") == 0) {
        umssedFound = true;
      }
    }
    //To check if the main processes are found on the list
    BOOST_REQUIRE(imssedFound ==  true);
    BOOST_REQUIRE(umssedFound ==  true);
    BOOST_REQUIRE(tmssedFound ==  true);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//IA1-E1:  Get the running processes with bad machine Id
// Get the running processes: bad machine Id
BOOST_AUTO_TEST_CASE(get_running_processes_bad_machine_Id_call) 
{

  BOOST_TEST_MESSAGE("Use case IA1-E1: Get the running processes with bad machine Id");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List processes
  IMS_Data::ListProcesses listProcess;
  IMS_Data::ProcessOp op;
  op.setMachineId(badMachineId);

  BOOST_CHECK_THROW(getProcesses(sessionKey, listProcess, op), VishnuException);
}

//IA1-E2:  Get the running processes for no admin user
// Get the running processes: no admin user
BOOST_AUTO_TEST_CASE(get_running_processes_no_admin_user_call) 
{

  BOOST_TEST_MESSAGE("Use case IA1-E2: Get the running processes for no admin user Id");
  //no admin user
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List processes
  IMS_Data::ListProcesses listProcess;
  IMS_Data::ProcessOp op;
  op.setMachineId(machineId);

  BOOST_CHECK_THROW(getProcesses(sessionKey, listProcess, op), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
