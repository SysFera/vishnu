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




//Test category 4
BOOST_FIXTURE_TEST_SUITE(get_metric_data, IMSSeDFixture)

// I2 - B : I2 - B: Get metric data normal
// Get metric data : normal call
BOOST_AUTO_TEST_CASE(get_metric_data_normal_call)
{

  BOOST_TEST_MESSAGE("Use case I2 - B: Get metric data");
  int nbResMetric = 2;
  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Metric
  IMS_Data::ListMetric list;
  IMS_Data::MetricHistOp op;
  //Set FREEMEMORY metric
  op.setType(3);

  try {
    //Wait for metric recording in database
    //10 represents the update frequency
    sleep ((nbResMetric*10)+5);
    BOOST_CHECK_EQUAL(getMetricHistory(sessionKey, m_test_ums_user_vishnu_machineid, list, op),0  );
    BOOST_MESSAGE("SIZE of list: " + convertToString(list.getMetric().size()));
    //FIXE BOOST_REQUIRE(list.getMetric().size() == nbResMetric);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// I2 - E1: Get metric data with bad machine identifier
// Get metric data : bad machine Id
BOOST_AUTO_TEST_CASE(get_metric_data_bad_machine_Id_call)
{

  BOOST_TEST_MESSAGE("Use case I2 - E1: Get metric data with bad machine Id");
  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Metric
  IMS_Data::ListMetric list;
  IMS_Data::MetricHistOp op;
  std::string  badMachineId="unknown_name";

  BOOST_CHECK_THROW(getMetricHistory(sessionKey, badMachineId, list, op), VishnuException);
}

// I2 - E2: Get metric data with bad machine identifier
// Get metric data : bad metric type

BOOST_AUTO_TEST_CASE(get_metric_data_bad_metric_type)
{

  BOOST_TEST_MESSAGE("Use case I2 - E2: Get metric data with bad metric type");
  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List Metric
  IMS_Data::ListMetric list;
  IMS_Data::MetricHistOp op;
  //Set unknown metric type
  op.setType(15);

  BOOST_CHECK_THROW(getMetricHistory(sessionKey, m_test_ums_user_vishnu_machineid, list, op), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
