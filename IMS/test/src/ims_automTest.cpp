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


//Test category 3

// I2 - B : I2 – B: Get metric  data

BOOST_AUTO_TEST_SUITE(get_metric_data)

  // Get metric data : normal call

BOOST_AUTO_TEST_CASE( get_metric_data_normal_call) {

  string sqlPath = IMSSQLPATH;

  BOOST_TEST_MESSAGE("\n\n Use case I2 – B: Get metric  data" << "\n");
  int nbResMetric = 2;
  VishnuConnexion vc("root","vishnu_user");
  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="machine_1";
  IMS_Data::ListMetric list;
  IMS_Data::MetricHistOp op;
  //Set the type of metric to get
  op.setType(5);

  //Initialize the databatase: put the update frequency to 10
  //and cleaning of the database state
  if (restore(sqlPath + "/IMSTestGetMetric.sql") != 0) {
    cout << "Database update failed" << endl;
    return;
  }
  sleep (5);
  //Wait for metric recording in database
  //10 represents the update frequency
  sleep (nbResMetric*10);

  BOOST_CHECK_EQUAL(getMetricHistory(sessionKey, machineId, list, op),0  );
  BOOST_TEST_MESSAGE( "list.getMetric().size()" << list.getMetric().size() << "\n");;

  BOOST_REQUIRE(list.getMetric().size() == nbResMetric);

 // BOOST_TEST_MESSAGE("\n\n  Testing Use case I2 – B: Get metric  data" << "\n");
}

BOOST_AUTO_TEST_SUITE_END()


// THE END

