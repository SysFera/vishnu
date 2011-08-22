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


//Test category 4
BOOST_FIXTURE_TEST_SUITE(get_metric_data, IMSSeDFixture)

// I2 - B : I2 – B: Get metric data normal
// Get metric data : normal call
BOOST_AUTO_TEST_CASE(get_metric_data_normal_call) 
{

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
BOOST_AUTO_TEST_CASE(get_metric_data_bad_machine_Id_call)
{

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

BOOST_AUTO_TEST_CASE(get_metric_data_bad_metric_type)
{

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

BOOST_AUTO_TEST_SUITE_END()
// THE END

