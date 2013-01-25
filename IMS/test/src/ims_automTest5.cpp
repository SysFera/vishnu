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

static const string badMachineId="unknown_name";
static const string sshCmd =" ssh -o PasswordAuthentication=no ";

BOOST_FIXTURE_TEST_SUITE(get_update_frequency, IMSSeDFixture)

// I1 - B: Get the update frequency normal call
// Get the update frequency : normal call
BOOST_AUTO_TEST_CASE(get_update_frequency_normal_call)
{

  BOOST_TEST_MESSAGE("Use case I1 - B: Get the update frequency");

  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int frequency;

  try {

    BOOST_CHECK_EQUAL(getUpdateFrequency(sessionKey, frequency),0 );
    //Check if the frequency is equal to 10
    //FIXE BOOST_REQUIRE(frequency == 10);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
