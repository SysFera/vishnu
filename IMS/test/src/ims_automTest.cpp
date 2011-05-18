/**
 * \file ims_automTest
 * \brief Contains IMS API test implementation
 * References: VISHNU_D5_1b_IMS-PlanTests
 */

//UMS forward Headers
#include "UMS_Data_forward.hpp"

// IMS forward Headers
#include "IMS_Data_forward.hpp"
#include "IMS_fixtures.hpp"
#include "imsTestUtils.hpp"
#include "IMS_testconfig.h"

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

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
      cout << "Database update failed" << endl;
      return;
    }
    sleep (5);
    //Wait for metric recording in database
    //10 represents the update frequency
    sleep (nbResMetric*10);

    BOOST_CHECK_EQUAL(getMetricHistory(sessionKey, machineId, list, op),0  );
    //BOOST_TEST_MESSAGE( "list.getMetric().size()" << list.getMetric().size() << "\n");;
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
  //BOOST_TEST_MESSAGE("Testing Use case I2 – E1: Get metric data with bad machine Id" << "\n");
}

// I2 – E2: Get metric data with bad machine identifier
// Get metric data : bad metric type

BOOST_AUTO_TEST_CASE( get_metric_data_bad_metric_type) {

  BOOST_TEST_MESSAGE("Use case I2 – E2: Get metric data with bad metric type");
  VishnuConnexion vc("root","vishnu_user");
  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="unknown_name";

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
    //BOOST_TEST_MESSAGE( "list.getMetric().size()" << list.getMetric().size() << "\n");
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
      cout << "Database update failed" << endl;
      return;
    }

    list.getThreshold().clear();

    BOOST_CHECK_EQUAL(getSystemThreshold(sessionKey, list, op),0 );
    //To check if the list is not empty
    BOOST_REQUIRE(list.getThreshold().size() != 0);
    //To check if the threshold is equal to 1000000
    BOOST_TEST_MESSAGE("list.getThreshold().get(0)->getValue()" << list.getThreshold().get(0)->getValue());
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

BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_machine_Id) {

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

BOOST_AUTO_TEST_CASE(get_system_load_threshold_bad_metric) {

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

BOOST_AUTO_TEST_CASE(get_system_load_threshold_for_no_admin_user) {

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

BOOST_AUTO_TEST_SUITE_END()
// THE END

