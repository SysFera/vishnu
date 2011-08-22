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

BOOST_FIXTURE_TEST_SUITE(set_system_info, IMSSeDFixture)

//IA-5: Set system info normal call
//Set system info: normal call
BOOST_AUTO_TEST_CASE(set_system_info_normal_call)
{

  BOOST_TEST_MESSAGE("Use case IA-5: Set system info normal call");

  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List System info
  IMS_Data::ListSysInfo listSysInfo;
  IMS_Data::SysInfoOp sysInfoOp;
  //To set the machineId
  sysInfoOp.setMachineId(machineId);
  //To set the SystemInfo values
  IMS_Data::SystemInfo systemInfo;
  systemInfo.setMachineId(machineId);
  systemInfo.setDiskSpace(5000000);
  systemInfo.setMemory(512);

  try {
    //Set the memory and the diskspace of machine_1 respectively to 512 and 5000000
    BOOST_CHECK_EQUAL(setSystemInfo(sessionKey, systemInfo),0  );
    //To get the values
    BOOST_CHECK_EQUAL(getSystemInfo(sessionKey, listSysInfo, sysInfoOp),0  );
    //If if the list is not empty
    BOOST_REQUIRE(listSysInfo.getSysInfo().size() > 0);
    //To check the memory
    BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getMemory() == 512);
    //To check the diskspace
    BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getDiskSpace() == 5000000);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//IA5-E1: Set system info with bad machine Id call
//Set system info: bad machine Id
BOOST_AUTO_TEST_CASE(set_system_info_bad_machine_Id_call) 
{

  BOOST_TEST_MESSAGE("Use case IA5-E1: Set system info with bad machine Id call");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //To set the SystemInfo values
  IMS_Data::SystemInfo systemInfo;
  systemInfo.setMachineId(badMachineId);
  systemInfo.setDiskSpace(5000000);
  systemInfo.setMemory(512);

  BOOST_CHECK_THROW(setSystemInfo(sessionKey, systemInfo), VishnuException);
}

//IA5-E1: Set system info with bad metric value call
//Set system info: bad metric value call
BOOST_AUTO_TEST_CASE(set_system_info_bad_mertric_value_call)
{

  BOOST_TEST_MESSAGE("Use case IA5-E2: Set system info with bad metric value call");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //To set the SystemInfo values
  IMS_Data::SystemInfo systemInfo;
  systemInfo.setMachineId(badMachineId);
  systemInfo.setDiskSpace(-5000000);
  systemInfo.setMemory(-512);

  BOOST_CHECK_THROW(setSystemInfo(sessionKey, systemInfo), VishnuException);
}

//IA5-E2: Set system info for no admin user
//Set system info: no admin user
BOOST_AUTO_TEST_CASE(set_system_info_no_admin_user_call) 
{

  BOOST_TEST_MESSAGE("Use case IA5-E2: Set system info for no admin user");
  //no admin user
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //To set the SystemInfo values
  IMS_Data::SystemInfo systemInfo;
  systemInfo.setMachineId(machineId);
  systemInfo.setDiskSpace(5000000);
  systemInfo.setMemory(512);

  BOOST_CHECK_THROW(setSystemInfo(sessionKey, systemInfo), VishnuException);
}
BOOST_AUTO_TEST_SUITE_END()
// THE END
