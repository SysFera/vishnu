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



// I4 - B: Get data on the infrastructure
BOOST_FIXTURE_TEST_SUITE(get_metric_current_value, IMSSeDFixture)

// Get metric data : normal call
BOOST_AUTO_TEST_CASE(get_metric_current_value)
{

  BOOST_TEST_MESSAGE("Use case I4 - B: Get data on the infrastructure");
  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List metric
  IMS_Data::ListMetric list;
  IMS_Data::CurMetricOp op;
  op.setMetricType(2);

  try {
    BOOST_CHECK_EQUAL(getMetricCurrentValue(sessionKey, m_test_ums_user_vishnu_machineid, list, op),0 );
    BOOST_REQUIRE(list.getMetric().size() != 0);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
