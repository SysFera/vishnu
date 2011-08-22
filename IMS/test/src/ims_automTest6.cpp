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

BOOST_FIXTURE_TEST_SUITE(set_update_frequency, IMSSeDFixture)

// IA6 – B: Set the update frequency normal call
// Get the update frequency : normal call
BOOST_AUTO_TEST_CASE(set_update_frequency_normal_call) 
{

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
BOOST_AUTO_TEST_CASE(set_update_frequency_zero_call) 
{

  BOOST_TEST_MESSAGE("Use case IA6 – E1: Set the update frequency with frequency equal to zero");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int frequency = 0;

  BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
}

// IA6 – E2: Set the update frequency with negative frequency
// Get the update frequency : negative frequency
BOOST_AUTO_TEST_CASE(set_update_frequency_negative_call) 
{

  BOOST_TEST_MESSAGE("Use case IA6 – E2: Set the update frequency with negative value");
  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  int frequency = -15;

  BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, frequency), VishnuException);
}

// IA6 – E3: Set the update frequency for no admin user
// Get the update frequency : no admin user
BOOST_AUTO_TEST_CASE(set_update_frequency_no_admin_call)
{

  BOOST_TEST_MESSAGE("Use case IA6 – E3: Set the update frequency for no admin user");
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();

  BOOST_CHECK_THROW(setUpdateFrequency(sessionKey, 15), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
