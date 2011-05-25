/**
 * \file ims_automTest
 * \brief Contains IMS API test implementation
 * References: VISHNU_D5_1b_IMS-PlanTests
 */

//UMS forward Headers
#include "UMS_Data_forward.hpp"
//IMS forward Headers
#include "IMS_Data_forward.hpp"

#include "IMS_fixtures.hpp"
#include "imsTestUtils.hpp"
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


// The database, UMS and IMS SeD are launched by IMSSedFixture.


BOOST_GLOBAL_FIXTURE(IMSSeDFixture)

BOOST_AUTO_TEST_SUITE(Information_Managment_System_test)
  //Test category 4
  // I2 - B : I2 – B: Get metric data normal
  // Get metric data : normal call

  BOOST_AUTO_TEST_CASE( get_metric_data_normal_call) {

    string sqlPath = IMSSQLPATH;

    BOOST_TEST_MESSAGE("Use case I2 – B: Get metric  data");
    int nbResMetric = 2;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    IMS_Data::ListMetric list;
    IMS_Data::MetricHistOp op;
    //Set FREEMOMORY metric
    op.setType(5);

    try {
      //Set the update frequency to 10 and clean the table state
      if (restore(sqlPath + "/IMSTestGetMetric.sql") != 0) {
        BOOST_TEST_MESSAGE("Database update failed for restore(sqlPath + /IMSTestGetMetric.sql)");
      }
      //Wait for metric recording in database
      //10 represents the update frequency
      sleep ((nbResMetric*10)+5);

      BOOST_CHECK_EQUAL(getMetricHistory(sessionKey, machineId, list, op),0  );
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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    IMS_Data::ListMetric list;
    IMS_Data::MetricHistOp op;

    BOOST_CHECK_THROW(getMetricHistory(sessionKey, machineId, list, op), VishnuException);
  }

  // I2 – E2: Get metric data with bad machine identifier
  // Get metric data : bad metric type

  BOOST_AUTO_TEST_CASE( get_metric_data_bad_metric_type) {

    BOOST_TEST_MESSAGE("Use case I2 – E2: Get metric data with bad metric type");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    IMS_Data::ListMetric list;
    IMS_Data::CurMetricOp op;
    op.setMetricType(3);

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
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    op.setMachineId(machineId);
    //Set FreeDiskSpace Metric
    op.setMetricType(3);

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
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    op.setMachineId(machineId);

    BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
  }

  // IA2.1 – E2 : Get a system load threshold with bad metric
  // Get a system load threshold : with bad metric

  BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_metric_call) {

    BOOST_TEST_MESSAGE("Use case IA2.1 – E2: Get a system load threshold with bad metric");
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    op.setMachineId(machineId);
    op.setMetricType(15);

    BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
  }

  // IA2.1 – E3 : Get a system load threshold for no admin user
  // Get a system load threshold : for no admin user

  BOOST_AUTO_TEST_CASE(get_system_load_threshold_for_no_admin_user_call) {

    BOOST_TEST_MESSAGE("Use case IA2.1 – E2: Get a system load threshold with no admin user");
    VishnuConnexion vc("user_1","toto");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    op.setMachineId(machineId);

    BOOST_CHECK_THROW(getSystemThreshold(sessionKey, list, op), VishnuException);
  }


  // IA2 – B1 : Define a system load threshold for CPUUSE Metric
  // Define a system load threshold : normal call

  BOOST_AUTO_TEST_CASE(define_system_load_threshold_normal_call_metric_CPU_USE) {

    BOOST_TEST_MESSAGE("Use case IA2 – B1: Define a system load threshold for CPUUSE metric");
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    int currentThreshold;

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
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    int currentThreshold;

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    IMS_Data::Threshold systemThreshold;

    //Set FREEDISKSPACE Metric
    op.setMetricType(3);
    op.setMachineId(machineId);
    //Set handler
    systemThreshold.setHandler("root");
    systemThreshold.setMachineId(machineId);
    //Set FREEDISKSPACE Metric
    systemThreshold.setType(3);

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
      BOOST_CHECK_EQUAL(setSystemThreshold(sessionKey, systemThreshold),0 );

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
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    int currentThreshold;

    IMS_Data::ListThreshold list;
    IMS_Data::ThresholdOp op;
    IMS_Data::Threshold systemThreshold;

    //Set FREEMEMORY Metric
    op.setMetricType(5);
    op.setMachineId(machineId);

    //Set handler
    systemThreshold.setHandler("root");
    systemThreshold.setMachineId(machineId);
    //Set FREEMEMORY Metric
    systemThreshold.setType(5);

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
    VishnuConnexion vc("root","vishnu_user");

    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    IMS_Data::ListThreshold list;
    IMS_Data::Threshold systemThreshold;
    //Set handler
    systemThreshold.setHandler("root");
    systemThreshold.setMachineId(machineId);
    //Set FREEMEMORY Metric
    systemThreshold.setType(5);

    BOOST_CHECK_THROW(setSystemThreshold(sessionKey, systemThreshold), VishnuException);
  }

  // IA2 – E2 : Define a system load threshold with bad machine Id
  // Define a system load threshold : bad machine Id

  BOOST_AUTO_TEST_CASE(define_system_load_threshold_bad_metric_call) {

    BOOST_TEST_MESSAGE("Use case IA2 – E2: Define a system load threshold with bad metric");
    VishnuConnexion vc("root","vishnu_user");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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
    VishnuConnexion vc("user_1","toto");

    string sqlPath = IMSSQLPATH;
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    IMS_Data::ListThreshold list;
    IMS_Data::Threshold systemThreshold;
    //Set handler
    systemThreshold.setHandler("root");
    systemThreshold.setMachineId(machineId);
    //Set FREEMEMORY Metric
    systemThreshold.setType(5);

    BOOST_CHECK_THROW(setSystemThreshold(sessionKey, systemThreshold), VishnuException);
  }


  // I1 – B: Get the update frequency normal call
  // Get the update frequency : normal call

  BOOST_AUTO_TEST_CASE( get_update_frequency_normal_call) {

    BOOST_TEST_MESSAGE("Use case I1 – B: Get the update frequency");
    string sqlPath = IMSSQLPATH;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
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
    string sqlPath = IMSSQLPATH;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    int frequency = 0;

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
  }

  // IA6 – E2: Set the update frequency with negative frequency
  // Get the update frequency : negative frequency
  BOOST_AUTO_TEST_CASE(set_update_frequency_negative_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – E2: Set the update frequency with negative value");
   VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    int frequency = -15;

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
  }

  // IA6 – E3: Set the update frequency for no admin user
  // Get the update frequency : no admin user
  BOOST_AUTO_TEST_CASE(set_update_frequency_no_admin_call) {

    BOOST_TEST_MESSAGE("Use case IA6 – E3: Set the update frequency");
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, 15), VishnuException);
  }

  //IA3-B: Define the identifier normal call
  //Define User/Machine/Job/FileTransfer identifier: normal call
   BOOST_AUTO_TEST_CASE( define_identifier_normal_call) {

    BOOST_TEST_MESSAGE("Use case IA3 – B: Define the identifier");
    string sqlPath = IMSSQLPATH;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    string formatUser = "UTEST_$CPT";
    string formatMachine = "MTEST_$CPT";
    string formatJob = "JTEST_$CPT";
    string formatFileTransfer = "FTTEST_$CPT";

    // CREATE DATA MODEL
    UMS_DataFactory_ptr ecoreUMSFactory = UMS_DataFactory::_instance();

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
      User_ptr user  = ecoreUMSFactory->createUser();
      user->setFirstname("TestFirstname");
      user->setLastname ("TestLastname");
      user->setPrivilege(0) ;
      user->setEmail    ("Test@test.com");
      BOOST_CHECK_EQUAL(addUser(sessionKey, *user), 0);
      //To check if the userId format is correct
      BOOST_REQUIRE(user->getUserId().compare("UTEST_1") == 0);

      //machine
      Machine_ptr ma  = ecoreUMSFactory->createMachine();
      ma->setName              ("mana");
      ma->setSite              ("site");
      ma->setMachineDescription("desc");
      ma->setLanguage          ("lang");
      ma->setSshPublicKey      ("/id_rsa.pub");

      BOOST_CHECK_EQUAL(addMachine(sessionKey, *ma), 0);
      //To check if the machineId format is correct
      BOOST_REQUIRE(ma->getMachineId().compare("MTEST_1") == 0);

      //Job
      const std::string scriptFilePath= TMSSCRIPTSPATH "/torque_script";
      Job jobInfo;
      SubmitOptions subOptions;

      BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0 );
      //To check if the jobId format is correct
      BOOST_REQUIRE(jobInfo.getJobId().compare("JTEST_1") == 0);

      //TODO: vishnu_ copyFile + check of transfer ID

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

    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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
    string sqlPath = IMSSQLPATH;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    IMS_Data::ListSysInfo listSysInfo;
    IMS_Data::SysInfoOp sysInfoOp;
    //To set the machineId
    sysInfoOp.setMachineId(machineId);

    BOOST_CHECK_THROW(getSystemInfo(sessionKey, listSysInfo, sysInfoOp), VishnuException);
  }

   //IA-5: Set system info normal call
   //Set system info: normal call
   BOOST_AUTO_TEST_CASE( set_system_info_normal_call) {

    BOOST_TEST_MESSAGE("Use case IA-5: Set system info normal call");
    string sqlPath = IMSSQLPATH;
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
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
      //BOOST_REQUIRE(listSysInfo.getSysInfo().size() != 0);
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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    //To set the SystemInfo values
    IMS_Data::SystemInfo systemInfo;
    systemInfo.setMachineId(machineId);
    systemInfo.setDiskSpace(5000000);
    systemInfo.setMemory(512);

    BOOST_CHECK_THROW(setSystemInfo(sessionKey, systemInfo), VishnuException);
  }

  //IA5-E1: Set system info with bad metric value call
  //Set system info: bad metric value call
  BOOST_AUTO_TEST_CASE(set_system_info_bad_mertric_value_call) {

    BOOST_TEST_MESSAGE("Use case IA5-E2: Set system info with bad metric value call");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    //To set the SystemInfo values
    IMS_Data::SystemInfo systemInfo;
    systemInfo.setMachineId(machineId);
    systemInfo.setDiskSpace(-5000000);
    systemInfo.setMemory(-512);

    BOOST_CHECK_THROW(setSystemInfo(sessionKey, systemInfo), VishnuException);
  }

  //IA5-E2: Set system info for no admin user
  //Set system info: no admin user
  BOOST_AUTO_TEST_CASE(set_system_info_no_admin_user_call) {

    BOOST_TEST_MESSAGE("Use case IA5-E2: Set system info for no admin user");
    //no admin user
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
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
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    IMS_Data::ListProcesses listProcess;
    IMS_Data::ProcessOp op;
    op.setMachineId(machineId);

    BOOST_CHECK_THROW(getProcesses(sessionKey, listProcess, op), VishnuException);
  }

  //IA1-E2:  Get the running processes for no admin user
  // Get the running processes: no admin user
  BOOST_AUTO_TEST_CASE(get_running_processes_no_admin_user_call) {

    BOOST_TEST_MESSAGE("Use case IA1-E2: Get the running processes for no admin user Id");
    //no admin user
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

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

    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    //Set loadShedType to 1: SOFT
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
      sleep(3);
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

  //IA4.1-E1 : Soft load schedding with bad machine Id
  //Soft load schedding: bad machine Id
  BOOST_AUTO_TEST_CASE(load_schedding_soft_bad_machine_Id_call) {

    BOOST_TEST_MESSAGE("Use case IA4.1-E1: Soft load schedding with bad machine Id");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";
    //Set loadShedType to 1: SOFT
    IMS_Data::LoadShedType loadShedType = 2;

    BOOST_CHECK_THROW(loadShed(sessionKey, machineId, loadShedType), VishnuException);
  }

  //IA4.1-E2 : Soft load schedding for no admin user
  //Soft load schedding: no admin user
  BOOST_AUTO_TEST_CASE(load_schedding_soft_no_admin_user_call) {

    BOOST_TEST_MESSAGE("Use case IA4.1-E2: Soft load schedding for no admin user");
    //no admin user
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    //Set loadShedType to 1: SOFT
    IMS_Data::LoadShedType loadShedType = 2;

    BOOST_CHECK_THROW(loadShed(sessionKey, machineId, loadShedType), VishnuException);
  }

  //TODO: Kévine ==> Ajouter use case dans Specs Genérales
  //IA9-B:  Stop normal call
  //Stop: normal call
  BOOST_AUTO_TEST_CASE( stop_normal_call ) {

    BOOST_TEST_MESSAGE("Use case IA9-B: Stop normal call");
    bool umssedFound = false;

    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    //The distant machine on which the process UMS will be stopped
    string machineId="machine_2";
    IMS_Data::ListProcesses listProcess;
    IMS_Data::ProcessOp op;
    op.setMachineId(machineId);

    //The process to stop
    IMS_Data::Process process;
    process.setMachineId(machineId);
    process.setProcessName("UMS");

    try {
      BOOST_CHECK_EQUAL(getProcesses(sessionKey, listProcess, op),0  );
      BOOST_REQUIRE(listProcess.getProcess().size() != 0);

      //To check if the process UMS exists
      for(unsigned int i = 0; i < listProcess.getProcess().size(); i++) {
        //To check umssed process
        if (listProcess.getProcess().get(i)->getProcessName().compare("UMS") == 0) {
          umssedFound = true;
        }
      }
      //To clear the list
      listProcess.getProcess().clear();
      //To check if the process ums has been found on the list of machine_2
      BOOST_REQUIRE(umssedFound ==  true);

      //To stop the process
      BOOST_CHECK_EQUAL(stop(sessionKey, process), 0);
      BOOST_CHECK_EQUAL(getProcesses(sessionKey, listProcess, op),0  );
      BOOST_REQUIRE(listProcess.getProcess().size() != 0);

      umssedFound = false;

      //To check if the process UMS is stopped and not on the list
      for(unsigned int i = 0; i < listProcess.getProcess().size(); i++) {
        //To check umssed process
        if (listProcess.getProcess().get(i)->getProcessName().compare("UMS") == 0) {
          umssedFound = true;
        }
      }
      //To check if the process ums has not been found on the list because it is stopped
      BOOST_REQUIRE(umssedFound ==  false);
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
  //IA9-E1:  Stop with bad machineId
  //Stop: with bad machine Id
  BOOST_AUTO_TEST_CASE(stop_bad_machine_Id_call) {

    BOOST_TEST_MESSAGE("Use case IA9-E1: Stop with bad machine Id call");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="unknown_name";

    //The process to stop
    IMS_Data::Process process;
    process.setMachineId(machineId);
    process.setProcessName("UMS");

    BOOST_CHECK_THROW(stop(sessionKey, process), VishnuException);
  }

  //IA9-E1:  Stop for no admin user
  //Stop: for no admin user
  BOOST_AUTO_TEST_CASE(stop_no_admin_user_call) {
    BOOST_TEST_MESSAGE("Use case IA9-E1: Stop for no admin user call");
    //no admin user
    VishnuConnexion vc("user_1","toto");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";

    //The process to stop
    IMS_Data::Process process;
    process.setMachineId(machineId);
    process.setProcessName("UMS");

    BOOST_CHECK_THROW(stop(sessionKey, process), VishnuException);
  }
  /*
  //Test category 1
  //I3-B: export and replay commands
  BOOST_AUTO_TEST_CASE( export_command_normal_call) {

    BOOST_TEST_MESSAGE("Use case I3 – B: Export and replay commands");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();
    string machineId="machine_1";
    // CREATE DATA MODEL
    UMS_DataFactory_ptr ecoreFactory = UMS_DataFactory::_instance();
    // Command history
    ListCommands_ptr listCmd  = ecoreFactory->createListCommands();
    ListCmdOptions listCmdOpt ;
    // List Sessions
    ListSessions_ptr listSess   = ecoreFactory->createListSessions();
    ListSessionOptions listSessionsOpt;
    //Options for export
    IMS_Data::ExportOp exportOp;
    //1 for Shell export
    exportOp.setExportType(1);
    string filename = "tmpTest.txt";
    string fileContent;
    try {
      //Options to get inactive sessions
      listSessionsOpt.setStatus(0);
      //To get the list of inactive sessions
      BOOST_CHECK_EQUAL(listSessions(sessionKey, *listSess, listSessionsOpt), 0);
      BOOST_REQUIRE(listSess->getSessions().size() != 0);
      //To get the sessionId of the first element of the list
      listCmdOpt.setSessionId(listSess->getSessions().get(0)->getSessionId());
      BOOST_CHECK_EQUAL(listHistoryCmd(sessionKey, *listCmd, listCmdOpt), 0);
      //At least one command registered
      BOOST_REQUIRE(listCmd->getCommands().size() > 1);

      //To create a locate file for the test
      std::ofstream file(filename.c_str());

      boost::filesystem3::path filePath (boost::filesystem3::current_path().string() +"/"+ filename);
      BOOST_TEST_MESSAGE("filePath.filename():" << filePath.filename());
      exportCommands(sessionKey, listCmdOpt.getSessionId(), filePath.filename().string(), exportOp);

      fileContent = vishnu::get_file_content(filePath.filename().c_str());
      BOOST_TEST_MESSAGE("FileContent:" << fileContent);

      //To remove the temporary file
      boost::filesystem3::remove(filePath);

      delete listSess;
      delete listCmd;
    }
    catch (VishnuException& e) {
      BOOST_MESSAGE("FAILED\n");
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
      delete listSess;
      delete listCmd;
    }
  }
  */
  //To clean the table process
  BOOST_AUTO_TEST_CASE( clean_table_process_call) {

    string sqlPath = IMSSQLPATH;
    BOOST_TEST_MESSAGE("Clean process table");
    VishnuConnexion vc("root","vishnu_user");
    // get the session key and the machine identifier
    string sessionKey=vc.getConnexion();

    try {
      //TODO:A mettre dans le dernier cas de tests
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

