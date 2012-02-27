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

BOOST_FIXTURE_TEST_SUITE(define_identifier, IMSSeDFixture)

//IA3-B: Define the identifier normal call
//Define User/Machine/Job/FileTransfer identifier: normal call
BOOST_AUTO_TEST_CASE(define_identifier_normal_call)
{

  BOOST_TEST_MESSAGE("Use case IA3 – B: Define the identifier");

  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();

  string formatUser = "UTEST_$CPT";
  string formatMachine = "MTEST_$CPT";
  string formatJob = "JTEST_$CPT";
  string formatFileTransfer = "FTTEST_$CPT";
  //Job
  const std::string scriptFilePath= TMSSCRIPTPATH;
  SubmitOptions subOptions;

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
    User user;
    user.setFirstname("TestFirstname");
    user.setLastname ("TestLastname");
    user.setPrivilege(0) ;
    user.setEmail    ("Test@test.com");
    BOOST_CHECK_EQUAL(addUser(sessionKey, user), 0);
    //To check if the userId format is correct
    BOOST_REQUIRE(user.getUserId().find("UTEST_") == 0);
    //machine
    Machine ma;//  = ecoreUMSFactory->createMachine();
    ma.setName              ("mana");
    ma.setSite              ("site");
    ma.setMachineDescription("desc");
    ma.setLanguage          ("lang");
    ma.setSshPublicKey      ("/id_rsa.pub");
    BOOST_CHECK_EQUAL(addMachine(sessionKey, ma), 0);
    //To check if the machineId format is correct
    BOOST_REQUIRE(ma.getMachineId().find("MTEST_") == 0);
    //job
    Job jobInfo;
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0 );
    //To check if the jobId format is correct
    BOOST_REQUIRE(jobInfo.getJobId().find("JTEST_") == 0);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//IA3-E1: Define the identifier with bad format
//Define User/Machine/Job/FileTransfer identifier: bad format
BOOST_AUTO_TEST_CASE(define_identifier_bad_format_call)
{

  BOOST_TEST_MESSAGE("Use case IA3 – E1: Define the identifier with bad format");
  //bad Format with unknwon $TEST
  string formatUser = "UTEST_$TEST";
  string formatMachine = "MTEST_$TEST";
  string formatJob = "JTEST_$TEST";
  string formatFileTransfer = "FTTEST_$TEST";

  VishnuConnection vc(adminId, adminPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
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
BOOST_AUTO_TEST_CASE(define_identifier_no_admin_call)
{

  BOOST_TEST_MESSAGE("Use case IA3 – E2: Define the identifier for no admin user");
  //Format with unknwon $TEST
  string formatUser = "UTEST_$CPT";
  string formatMachine = "MTEST_$CPT";
  string formatJob = "JTEST_$CPT";
  string formatFileTransfer = "FTTEST_$CPT";
  //no admin user
  VishnuConnection vc(userId, userPwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();

  //To define the user format
  BOOST_CHECK_THROW(defineUserIdentifier(sessionKey, formatUser), VishnuException);
  //To define the machine format
  BOOST_CHECK_THROW(defineMachineIdentifier(sessionKey, formatMachine), VishnuException);
  //To define the job format
  BOOST_CHECK_THROW(defineJobIdentifier(sessionKey, formatJob), VishnuException);
  //To define the file transfer format
  BOOST_CHECK_THROW(defineTransferIdentifier(sessionKey, formatFileTransfer), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
