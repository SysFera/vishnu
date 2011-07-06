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

// The database, UMS and IMS SeD are launched by IMSSedFixture.
BOOST_GLOBAL_FIXTURE(IMSSeDFixture)

BOOST_AUTO_TEST_SUITE(Information_Managment_System_test)

  //Test category 4
  // I2 - B : I2 – B: Get metric data normal
  // Get metric data : normal call
  BOOST_AUTO_TEST_CASE( get_metric_data_normal_call) {

    BOOST_TEST_MESSAGE("Use case I2 – B: Get metric data");
    int nbResMetric = 2;
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //List Metric
    IMS_Data::ListMetric list;
    IMS_Data::MetricHistOp op;
    //Set FREEMEMORY metric
    op.setType(3);

    try {
      //Set the update frequency to 10 and clean the table state
      if (restore(sqlPath + "/IMSTestGetMetric.sql") != 0) {
        BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMSTestGetMetric.sql)");
      }
      //Wait for metric recording in database
      //10 represents the update frequency
      sleep ((nbResMetric*10)+5);
      BOOST_CHECK_EQUAL(getMetricHistory(sessionKey, machineId, list, op),0  );
      BOOST_MESSAGE("SIZE of list: " + convertToString(list.getMetric().size()));
      BOOST_REQUIRE(list.getMetric().size() == nbResMetric);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

  // I2 – E1: Get metric data with bad machine identifier
  // Get metric data : bad machine Id
  BOOST_AUTO_TEST_CASE( get_metric_data_bad_machine_Id_call) {

    BOOST_TEST_MESSAGE("Use case I2 – E1: Get metric data with bad machine Id");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //List Metric
    IMS_Data::ListMetric list;
    IMS_Data::MetricHistOp op;

    BOOST_CHECK_THROW(getMetricHistory(sessionKey, badMachineId, list, op), VishnuException);
  }

  // I2 – E2: Get metric data with bad machine identifier
  // Get metric data : bad metric type

  BOOST_AUTO_TEST_CASE( get_metric_data_bad_metric_type) {

    BOOST_TEST_MESSAGE("Use case I2 – E2: Get metric data with bad metric type");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //List Metric
    IMS_Data::ListMetric list;
    IMS_Data::MetricHistOp op;
    //Set unknown metric type
    op.setType(15);

    BOOST_CHECK_THROW(getMetricHistory(sessionKey, machineId, list, op), VishnuException);
  }



  // I4 – B: Get data on the infrastructure
  // Get metric data : normal call
  BOOST_AUTO_TEST_CASE(get_metric_current_value) {

    BOOST_TEST_MESSAGE("Use case I4 – B: Get data on the infrastructure");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //List metric
    IMS_Data::ListMetric list;
    IMS_Data::CurMetricOp op;
    op.setMetricType(2);

    try {
      BOOST_CHECK_EQUAL(getMetricCurrentValue(sessionKey, machineId, list, op),0 );
      BOOST_REQUIRE(list.getMetric().size() != 0);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }


  // IA2.1 – B : Get a system load threshold
  // Get a system load threshold : normal call
  BOOST_AUTO_TEST_CASE(get_system_load_threshold) {

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

  BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_machine_Id_call) {

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
  BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_metric_call) {

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

  BOOST_AUTO_TEST_CASE(get_system_load_threshold_for_no_admin_user_call) {

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


  // IA2 – B1 : Define a system load threshold for CPUUSE Metric
  // Define a system load threshold : normal call
  BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_CPU_USE) {

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
  BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_FREE_DISK_SPACE) {

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
  BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_FREE_MEMORY) {

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
  BOOST_AUTO_TEST_CASE(define_system_load_threshold_bad_machine_Id_call) {

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
  BOOST_AUTO_TEST_CASE(define_system_load_threshold_bad_metric_call) {
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

  BOOST_AUTO_TEST_CASE(define_system_load_threshold_no_admin_call) {

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


  // I1 – B: Get the update frequency normal call
  // Get the update frequency : normal call

  BOOST_AUTO_TEST_CASE( get_update_frequency_normal_call) {

    BOOST_TEST_MESSAGE("Use case I1 – B: Get the update frequency");

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    int frequency;

    try {
      //Set the update frequency to 10 and clean the table state
      if (restore(sqlPath + "/IMSTestGetMetric.sql") != 0) {
        BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMSTestGetMetric.sql)");
      }
      BOOST_CHECK_EQUAL(getUpdateFrequency(sessionKey, frequency),0 );
      //Check if the frequency is equal to 10
      BOOST_REQUIRE(frequency == 10);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

  // IA6 – B: Set the update frequency normal call
  // Get the update frequency : normal call

  BOOST_AUTO_TEST_CASE( set_update_frequency_normal_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – B: Set the update frequency");

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    int frequency;
    int newFrequence;

    try {
      BOOST_CHECK_EQUAL(getUpdateFrequency(sessionKey, frequency),0 );
      //To check if the frequency is positive
      BOOST_REQUIRE(frequency >= 0);
      //To add 10 to the current frequency value
      newFrequence = frequency + 10;
      //To set the new frequency value
      BOOST_CHECK_EQUAL(setUpdateFrequency(sessionKey, newFrequence),0 );
      //To re-initialize the frequency value
      frequency = 0;
      //To get the frequency value
      BOOST_CHECK_EQUAL(getUpdateFrequency(sessionKey, frequency),0 );
      //Check if the frequency is equal to the value of the new frequency
      BOOST_REQUIRE(frequency == newFrequence);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

  // IA6 – E1: Set the update frequency with frequency value equal to zero
  // Set the update frequency : frequency equal to zero
  BOOST_AUTO_TEST_CASE(set_update_frequency_zero_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – E1: Set the update frequency with frequency equal to zero");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    int frequency = 0;

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
  }

  // IA6 – E2: Set the update frequency with negative frequency
  // Get the update frequency : negative frequency
  BOOST_AUTO_TEST_CASE(set_update_frequency_negative_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – E2: Set the update frequency with negative value");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    int frequency = -15;

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
  }

  // IA6 – E3: Set the update frequency for no admin user
  // Get the update frequency : no admin user
  BOOST_AUTO_TEST_CASE(set_update_frequency_no_admin_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – E3: Set the update frequency for no admin user");
    VishnuConnection vc(userId, userPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, 15), VishnuException);
  }

  //IA3-B: Define the identifier normal call
  //Define User/Machine/Job/FileTransfer identifier: normal call
   BOOST_AUTO_TEST_CASE( define_identifier_normal_call) {

    BOOST_TEST_MESSAGE("Use case IA3 – B: Define the identifier");

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();

    string formatUser = "UTEST_$CPT";
    string formatMachine = "MTEST_$CPT";
    string formatJob = "JTEST_$CPT";
    string formatFileTransfer = "FTTEST_$CPT";
    //Job
    const std::string scriptFilePath= TMSSCRIPTSPATH "/torque_script";
    SubmitOptions subOptions;

    try {
      //To define the user format
      BOOST_CHECK_EQUAL(defineUserIdentifier(sessionKey, formatUser),0 );
      //To define the machine format
      BOOST_CHECK_EQUAL(defineMachineIdentifier(sessionKey, formatMachine),0 );
      //To define the job format
      BOOST_CHECK_EQUAL(defineJobIdentifier(sessionKey, formatJob),0 );
      //To define the file transfer format
      BOOST_CHECK_EQUAL(defineTransferIdentifier(sessionKey, formatFileTransfer),0 );
      //user
      User user;
      user.setFirstname("TestFirstname");
      user.setLastname ("TestLastname");
      user.setPrivilege(0) ;
      user.setEmail    ("Test@test.com");
      BOOST_CHECK_EQUAL(addUser(sessionKey, user), 0);
      //To check if the userId format is correct
      BOOST_REQUIRE(user.getUserId().compare("UTEST_1") == 0);
      //machine
      Machine ma;//  = ecoreUMSFactory->createMachine();
      ma.setName              ("mana");
      ma.setSite              ("site");
      ma.setMachineDescription("desc");
      ma.setLanguage          ("lang");
      ma.setSshPublicKey      ("/id_rsa.pub");
      BOOST_CHECK_EQUAL(addMachine(sessionKey, ma), 0);
      //To check if the machineId format is correct
      BOOST_REQUIRE(ma.getMachineId().compare("MTEST_1") == 0);
      //job
      Job jobInfo;
      BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0 );
      //To check if the jobId format is correct
      BOOST_REQUIRE(jobInfo.getJobId().compare("JTEST_1") == 0);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

  //IA3-E1: Define the identifier with bad format
  //Define User/Machine/Job/FileTransfer identifier: bad format
  BOOST_AUTO_TEST_CASE(define_identifier_bad_format_call) {

    BOOST_TEST_MESSAGE("Use case IA3 – E1: Define the identifier with bad format");
    //bad Format with unknwon $TEST
    string formatUser = "UTEST_$TEST";
    string formatMachine = "MTEST_$TEST";
    string formatJob = "JTEST_$TEST";
    string formatFileTransfer = "FTTEST_$TEST";

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //To define the user format
    BOOST_CHECK_THROW(defineUserIdentifier(sessionKey, formatUser), VishnuException);
    //To define the machine format
    BOOST_CHECK_THROW(defineMachineIdentifier(sessionKey, formatMachine), VishnuException);
    //To define the job format
    BOOST_CHECK_THROW(defineJobIdentifier(sessionKey, formatJob), VishnuException);
    //To define the file transfer format
    BOOST_CHECK_THROW(defineTransferIdentifier(sessionKey, formatFileTransfer), VishnuException);
  }

  //IA3-E2: Define the identifier for no admin user
  //Define User/Machine/Job/FileTransfer identifier: no admin user
  BOOST_AUTO_TEST_CASE(define_identifier_no_admin_call) {

    BOOST_TEST_MESSAGE("Use case IA3 – E2: Define the identifier for no admin user");
    //Format with unknwon $TEST
    string formatUser = "UTEST_$CPT";
    string formatMachine = "MTEST_$CPT";
    string formatJob = "JTEST_$CPT";
    string formatFileTransfer = "FTTEST_$CPT";
    //no admin user
    VishnuConnection vc(userId, userPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();

    //To define the user format
    BOOST_CHECK_THROW(defineUserIdentifier(sessionKey, formatUser), VishnuException);
    //To define the machine format
    BOOST_CHECK_THROW(defineMachineIdentifier(sessionKey, formatMachine), VishnuException);
    //To define the job format
    BOOST_CHECK_THROW(defineJobIdentifier(sessionKey, formatJob), VishnuException);
    //To define the file transfer format
    BOOST_CHECK_THROW(defineTransferIdentifier(sessionKey, formatFileTransfer), VishnuException);
  }

   //I5-B: Get system info normal call
   //Get system info: normal call
   BOOST_AUTO_TEST_CASE( get_system_info_normal_call) {

    BOOST_TEST_MESSAGE("Use case I5-B: Get system info normal call");

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    IMS_Data::ListSysInfo listSysInfo;
    IMS_Data::SysInfoOp sysInfoOp;
    //To set the machineId
    sysInfoOp.setMachineId(machineId);

    try {
      //Set the memory and the diskspace of machine_1 respectively to 256 and 1000000
      if (restore(sqlPath + "/IMSTestSystemInfo.sql") != 0) {
        BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMSTestSystemInfo.sql)");
      }

      BOOST_CHECK_EQUAL(getSystemInfo(sessionKey, listSysInfo, sysInfoOp),0  );
      //If if the list is not empty
      BOOST_REQUIRE(listSysInfo.getSysInfo().size() > 0);
      //To check the memory
      BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getMemory() == 256);
      //To check the diskspace
      BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getDiskSpace() == 1000000);
      //BOOST_REQUIRE(listSysInfo.getSysInfo().size() != 0);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
   //I5-B: Get system info with bad machine Id call
   //Get system info: bad machine Id
  BOOST_AUTO_TEST_CASE(get_system_info_bad_machine_Id_call) {

    BOOST_TEST_MESSAGE("Use case I5-B: Get system info with bad machine Id call");
    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //List System info
    IMS_Data::ListSysInfo listSysInfo;
    IMS_Data::SysInfoOp sysInfoOp;
    //To set the machineId
    sysInfoOp.setMachineId(badMachineId);

    BOOST_CHECK_THROW(getSystemInfo(sessionKey, listSysInfo, sysInfoOp), VishnuException);
  }

   //IA-5: Set system info normal call
   //Set system info: normal call
   BOOST_AUTO_TEST_CASE( set_system_info_normal_call) {

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
  BOOST_AUTO_TEST_CASE(set_system_info_bad_machine_Id_call) {

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
  BOOST_AUTO_TEST_CASE(set_system_info_bad_mertric_value_call) {

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
  BOOST_AUTO_TEST_CASE(set_system_info_no_admin_user_call) {

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
    //Test category 3
    //IA1-B:  Get the running processes normal call
   // Get the running processes: normal call
   BOOST_AUTO_TEST_CASE( get_running_processes_normal_call) {

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
  BOOST_AUTO_TEST_CASE(get_running_processes_bad_machine_Id_call) {

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
  BOOST_AUTO_TEST_CASE(get_running_processes_no_admin_user_call) {

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

  //Test category 2
  //IA4.1-B : Soft load schedding normal call
  //Soft load schedding: normal call
  BOOST_AUTO_TEST_CASE( load_schedding_soft_normal_call) {

    BOOST_TEST_MESSAGE("Use case IA4.1-B: Soft load schedding normal call");

    VishnuConnection vc(adminId, adminPwd);
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    //Set loadShedType to 2: SOFT
    IMS_Data::LoadShedType loadShedType = 2;
    //Job
    const std::string scriptFilePath= TMSSCRIPTSPATH "/torque_script";
    Job jobInfo1;
    Job jobInfo2;
    SubmitOptions subOptions;
    //ListJobs
    ListJobs lsJobs;
    ListJobsOptions lsOptions;

    try {
      //To submit 2 jobs
      BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo1,subOptions),0 );
      BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo2,subOptions),0 );
      //To list jobs
      BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId, lsJobs, lsOptions), 0);
      //To check that the jobs are submitted
      BOOST_REQUIRE(lsJobs.getJobs().size() != 0);
      //Launch load schedding SOFT
      BOOST_CHECK_EQUAL(loadShed(sessionKey, machineId, loadShedType), 0);
      lsJobs.getJobs().clear();
      //time to get the correct update of the Batch scheduler
      sleep(5);
      //To list jobs
      BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId, lsJobs, lsOptions), 0);
      //To check that the jobs are canceled
      BOOST_REQUIRE(lsJobs.getJobs().size() == 0);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

  //Test category 1
  //I3-B: export and replay commands
  BOOST_AUTO_TEST_CASE( export_command_normal_call) {

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

  //To clean the table process
  BOOST_AUTO_TEST_CASE( cleaning_call) {

    BOOST_TEST_MESSAGE("Cleaning call");
    VishnuConnection vc(adminId, adminPwd);
    int ret;
    // get the session key and the machine identifier
    string sessionKey=vc.getSessionKey();
    try {
      //Clean of the table process
      BOOST_CHECK_EQUAL(restore(sqlPath + "/IMScleanProcesses.sql"), 0);

      if (restore(sqlPath + "/IMScleanProcesses.sql") != 0) {
        BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMScleanProcesses.sql)");
      }
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
BOOST_AUTO_TEST_SUITE_END()
// THE END

