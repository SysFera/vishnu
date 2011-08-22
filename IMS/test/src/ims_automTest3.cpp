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

// IA2.1 – B : Get a system load threshold
BOOST_FIXTURE_TEST_SUITE(get_system_load_threshold, IMSSeDFixture)

// Get a system load threshold : normal call

BOOST_AUTO_TEST_CASE(get_system_load_threshold) 
{

  BOOST_TEST_MESSAGE("Use case IA2.1 – B: Get a system load threshold");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Threshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  op.setMachineId(machineId);
  //Set FreeDiskSpace Metric
  op.setMetricType(2);

  try {
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the threshold is equal to 2000000
    BOOST_REQUIRE(list.getThreshold().get(0)->getValue() == 2000000);
    //Set the Threshold to 1000000
    if (restore(sqlPath + "/IMSTestThreshold.sql") != 0) {
      BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMSTestThreshold.sql)");
    }
    //To clear the list
    list.getThreshold().clear();
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the threshold is equal to 1000000
    BOOST_REQUIRE(list.getThreshold().get(0)->getValue() == 1000000);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// IA2.1 – E1 : Get a system load threshold with bad machine Id
// Get a system load threshold : with bad machine Id
BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_machine_Id_call) 
{

  BOOST_TEST_MESSAGE("Use case IA2.1 – E1: Get a system load threshold with bad machine Id");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  op.setMachineId(badMachineId);

  BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
}

// IA2.1 – E2 : Get a system load threshold with bad metric
// Get a system load threshold : with bad metric
BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_metric_call) 
{

  BOOST_TEST_MESSAGE("Use case IA2.1 – E2: Get a system load threshold with bad metric");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Threshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  op.setMachineId(machineId);
  op.setMetricType(15);

  BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
}

// IA2.1 – E3 : Get a system load threshold for no admin user
// Get a system load threshold : for no admin user
BOOST_AUTO_TEST_CASE(get_system_load_threshold_for_no_admin_user_call)
{

  BOOST_TEST_MESSAGE("Use case IA2.1 – E3: Get a system load threshold with no admin user");
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Threshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  op.setMachineId(machineId);

  BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
