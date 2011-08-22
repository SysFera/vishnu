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


// IA2 – B1 : Define a system load threshold for CPUUSE Metric
BOOST_FIXTURE_TEST_SUITE(define_system_load_threshold, IMSSeDFixture)

// Define a system load threshold : normal call
BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_CPU_USE) 
{

  BOOST_TEST_MESSAGE("Use case IA2 – B1: Define a system load threshold for CPUUSE metric");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int currentThreshold;
  //List Threshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  IMS_Data::Threshold systemThreshold;
  op.setMachineId(machineId);
  //Set CPUUSE Metric
  op.setMetricType(1);
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(machineId);
  //Set CPUUSE Metric
  systemThreshold.setType(1);

  try {

    //To get the current value of the CPUUSE metric
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To get thr value of the current threshold value
    currentThreshold = list.getThreshold().get(0)->getValue();
    //To clean the list
    list.getThreshold().clear();
    //To change the value of the currentThreshold
    int newThreshold = currentThreshold + 10;
    //To set a new value for the CPUUSE Metric
    systemThreshold.setValue(newThreshold);
    BOOST_CHECK_EQUAL(setSystemThreshold(sessionKey, systemThreshold),0 );
    //To get the current value of the CPUUSE metric
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the new value for the metric is ok
    BOOST_REQUIRE(list.getThreshold().get(0)->getValue() == newThreshold);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// IA2 – B2 : Define a system load threshold for FREEDISKSPACE Metric
// Define a system load threshold : normal call
BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_FREE_DISK_SPACE)
{

  BOOST_TEST_MESSAGE("Use case IA2 – B2: Define a system load threshold for FREE DISK SPACE Metric");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int currentThreshold;
  //List Threshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  IMS_Data::Threshold systemThreshold;
  //Set FREEDISKSPACE Metric
  op.setMetricType(2);
  op.setMachineId(machineId);
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(machineId);
  //Set FREEDISKSPACE Metric
  systemThreshold.setType(2);

  try {

    //To get the current value of the FREEDISKSPACE metric
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To get thr value of the current threshold value
    currentThreshold = list.getThreshold().get(0)->getValue();
    //To clean the list
    list.getThreshold().clear();
    //To change the value of the currentThreshold
    int newThreshold = currentThreshold + 1000000;
    //To set a new value for the FREEDISKSPACE Metric
    systemThreshold.setValue(newThreshold);
    //To set a new threshold
    BOOST_CHECK_EQUAL(setSystemThreshold(sessionKey, systemThreshold),0 );
    //To get the threshold from the database
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the new value for the metric FREEDISKSPACE is ok
    BOOST_REQUIRE(list.getThreshold().get(0)->getValue() == newThreshold);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// IA2 – B3 : Define a system load threshold for FREEMEMORY Metric
// Define a system load threshold : normal call
BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_FREE_MEMORY)
{

  BOOST_TEST_MESSAGE("Use case IA2 – B3: Define a system load threshold for FREE MEMORY Metric");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int currentThreshold;
  //ListThreshold
  IMS_Data::ListThreshold list;
  IMS_Data::ThresholdOp op;
  IMS_Data::Threshold systemThreshold;
  //Set FREEMEMORY Metric
  op.setMetricType(3);
  op.setMachineId(machineId);
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(machineId);
  //Set FREEMEMORY Metric
  systemThreshold.setType(3);

  try {
    //To get the current value of the FREEMEMORY metric
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To get thr value of the current threshold value
    currentThreshold = list.getThreshold().get(0)->getValue();
    //To clean the list
    list.getThreshold().clear();
    //To change the value of the currentThreshold
    int newThreshold = currentThreshold + 10;
    //To set a new value for the FREEDISKSPACE Metric
    systemThreshold.setValue(newThreshold);
    BOOST_CHECK_EQUAL(setSystemThreshold(sessionKey, systemThreshold),0 );
    //To check the new value
    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the value for the metric is ok
    BOOST_REQUIRE(list.getThreshold().get(0)->getValue() == newThreshold);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// IA2 – E1 : Define a system load threshold with bad machine Id
// Define a system load threshold : bad machine Id
BOOST_AUTO_TEST_CASE(define_system_load_threshold_bad_machine_Id_call) 
{

  BOOST_TEST_MESSAGE("Use case IA2 – E1: Define a system load threshold with bad machine Id");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //ListThreshold
  IMS_Data::ListThreshold list;
  IMS_Data::Threshold systemThreshold;
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(badMachineId);
  //Set FREEMEMORY Metric
  systemThreshold.setType(3);

  BOOST_CHECK_THROW(setSystemThreshold(sessionKey, systemThreshold), VishnuException);
}

// IA2 – E2 : Define a system load threshold with bad machine Id
// Define a system load threshold : bad machine Id
BOOST_AUTO_TEST_CASE(define_system_load_threshold_bad_metric_call) 
{
  BOOST_TEST_MESSAGE("Use case IA2 – E2: Define a system load threshold with bad metric");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //ListThreshold
  IMS_Data::ListThreshold list;
  IMS_Data::Threshold systemThreshold;
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(machineId);
  //Set unknown metric
  systemThreshold.setType(15);

  BOOST_CHECK_THROW(setSystemThreshold(sessionKey, systemThreshold), VishnuException);
}

// IA2 – E3 : Define a system load threshold for no admin user
// Define a system load threshold : for no admin user

BOOST_AUTO_TEST_CASE(define_system_load_threshold_no_admin_call)
{

  BOOST_TEST_MESSAGE("Use case IA2 – E3: Define a system load threshold for no admin user");
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //ListThreshold
  IMS_Data::ListThreshold list;
  IMS_Data::Threshold systemThreshold;
  //Set handler
  systemThreshold.setHandler("root");
  systemThreshold.setMachineId(machineId);
  //Set FREEMEMORY Metric
  systemThreshold.setType(3);

  BOOST_CHECK_THROW(setSystemThreshold(sessionKey, systemThreshold), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
