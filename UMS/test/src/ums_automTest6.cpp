/**
 * \file ims_automTest
 * \brief Contains UMS API test implementation
 * References: VISHNU_D5_1b_UMS-PlanTests
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

//UMS
#include "UMS_Data.hpp"
//UMS forward Headers
#include "UMS_Data_forward.hpp"
#include "UMS_fixtures.hpp"
#include "api_ums.hpp"
#include "utilVishnu.hpp"
#include "DIET_client.h"
#include "UMS_test_utils.hpp"

#ifdef COMPILE_TMS
#include "TMS_Data.hpp"
using namespace TMS_Data;
#endif

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
using namespace UMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;


static const string badMachineId="unknown_name";
static const string sshCmd =" ssh -o PasswordAuthentication=no ";

BOOST_FIXTURE_TEST_SUITE(define_identifier, UMSSeDFixture)

// FMS ID IS NOT TESTED
// WORK ID IS NOT TESTED
// AUTH ID IS NOT TESTED

BOOST_AUTO_TEST_SUITE_END()
// THE END
