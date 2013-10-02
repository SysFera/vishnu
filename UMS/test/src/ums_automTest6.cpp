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

//IA3-B: Define the identifier normal call
//Define User/Machine/Job/FileTransfer identifier: normal call
BOOST_AUTO_TEST_CASE(define_identifier_normal_call)
{

  BOOST_TEST_MESSAGE("Use case IA3 - B: Define the identifier");

  Session sess;
  ConnectOptions cop;
  BOOST_CHECK_EQUAL(connect(m_test_ums_admin_vishnu_login, m_test_ums_admin_vishnu_pwd, sess, cop ), 0);
  // get the session key and the machine identifier
  string sessionKey=sess.getSessionKey();

  string formatUser = "UTEST_$CPT";
  string formatMachine = "MTEST_$CPT";
  string formatJob = "JTEST_$CPT";
  string formatFileTransfer = "FTTEST_$CPT";
  string formatAuth = "ATEST_$CPT";

  try {
    //To define the user format
    BOOST_CHECK_EQUAL(defineUserIdentifier(sessionKey, formatUser),0 );
    //To define the machine format
    BOOST_CHECK_EQUAL(defineMachineIdentifier(sessionKey, formatMachine),0 );
    //To define the job format
    BOOST_CHECK_EQUAL(defineJobIdentifier(sessionKey, formatJob),0 );
    //To define the file transfer format
    BOOST_CHECK_EQUAL(defineTransferIdentifier(sessionKey, formatFileTransfer),0 );
    //To define the auth system format
    BOOST_CHECK_EQUAL(defineAuthIdentifier(sessionKey, formatAuth),0 );
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
    //To check if the m_test_ums_user_vishnu_machineid format is correct
    BOOST_REQUIRE(ma.getMachineId().find("MTEST_") == 0);
#ifdef COMPILE_TMS
    //Job
    const std::string scriptFilePath= "TMSSCRIPTPATH"; //FIXE
    SubmitOptions subOptions;
    Job jobInfo;
    BOOST_CHECK_EQUAL(submitJob(sessionKey, m_test_ums_user_vishnu_machineid, scriptFilePath, jobInfo,subOptions),0 );
    //To check if the jobId format is correct
    BOOST_REQUIRE(jobInfo.getJobId().find("JTEST_") == 0);
#endif // COMPILE_TMS

  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//IA3-E2: Define the identifier for no admin user
//Define User/Machine/Job/FileTransfer identifier: no admin user
BOOST_AUTO_TEST_CASE(define_identifier_no_admin_call)
{

  BOOST_TEST_MESSAGE("Use case IA3 - E2: Define the identifier for no admin user");
  //Format with unknwon $TEST
  string formatUser = "UTEST_$CPT";
  string formatMachine = "MTEST_$CPT";
  string formatJob = "JTEST_$CPT";
  string formatFileTransfer = "FTTEST_$CPT";
  string formatAuth = "ATTEST_$CPT";
  //no admin user
  Session sess;
  ConnectOptions cop;
  BOOST_CHECK_EQUAL(connect(m_test_ums_user_vishnu_login, m_test_ums_user_vishnu_pwd, sess, cop ), 0);
  // get the session key and the machine identifier
  string sessionKey=sess.getSessionKey();

  //To define the user format
  BOOST_CHECK_THROW(defineUserIdentifier(sessionKey, formatUser), VishnuException);
  //To define the machine format
  BOOST_CHECK_THROW(defineMachineIdentifier(sessionKey, formatMachine), VishnuException);
  //To define the job format
  BOOST_CHECK_THROW(defineJobIdentifier(sessionKey, formatJob), VishnuException);
  //To define the file transfer format
  BOOST_CHECK_THROW(defineTransferIdentifier(sessionKey, formatFileTransfer), VishnuException);
  //To define the auth system format
  BOOST_CHECK_THROW(defineAuthIdentifier(sessionKey, formatAuth), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
