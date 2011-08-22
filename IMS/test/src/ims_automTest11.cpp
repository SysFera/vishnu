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

BOOST_FIXTURE_TEST_SUITE(load_schedding_soft, IMSSeDFixture)

//Test category 2
//IA4.1-B : Soft load schedding normal call
//Soft load schedding: normal call
BOOST_AUTO_TEST_CASE(load_schedding_soft_normal_call) 
{

  BOOST_TEST_MESSAGE("Use case IA4.1-B: Soft load schedding normal call");

  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //Set loadShedType to 2: SOFT
  IMS_Data::LoadShedType loadShedType = 2;
  //Job
  const std::string scriptFilePath= TMSSCRIPTPATH;
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

BOOST_AUTO_TEST_SUITE_END()
// THE END
