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

BOOST_FIXTURE_TEST_SUITE(get_system_info, IMSSeDFixture)

//I5-B: Get system info normal call
//Get system info: normal call
BOOST_AUTO_TEST_CASE(get_system_info_normal_call)
{

  BOOST_TEST_MESSAGE("Use case I5-B: Get system info normal call");

  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  IMS_Data::ListSysInfo listSysInfo;
  IMS_Data::SysInfoOp sysInfoOp;
  //To set the m_test_ums_user_vishnu_machineid
  sysInfoOp.setMachineId(m_test_ums_user_vishnu_machineid);

  try {

    BOOST_CHECK_EQUAL(getSystemInfo(sessionKey, listSysInfo, sysInfoOp),0  );
    //If if the list is not empty
    BOOST_REQUIRE(listSysInfo.getSysInfo().size() > 0);
    //To check the memory
    //FIXE BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getMemory() == 256);
    //To check the diskspace
    //FIXE BOOST_REQUIRE(listSysInfo.getSysInfo().get(0)->getDiskSpace() == 1000000);
    //BOOST_REQUIRE(listSysInfo.getSysInfo().size() != 0);
  }
  catch (VishnuException& e) {
    BOOST_MESSAGE("FAILED\n");
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}
//I5-B: Get system info with bad machine Id call
//Get system info: bad machine Id
BOOST_AUTO_TEST_CASE(get_system_info_bad_machine_Id_call) 
{

  BOOST_TEST_MESSAGE("Use case I5-B: Get system info with bad machine Id call");
  VishnuConnection vc(m_test_ims_admin_vishnu_login, m_test_ims_admin_vishnu_pwd);
  // get the session key and the machine identifier
  string sessionKey=vc.getSessionKey();
  //List System info
  IMS_Data::ListSysInfo listSysInfo;
  IMS_Data::SysInfoOp sysInfoOp;
  //To set the m_test_ums_user_vishnu_machineid
  sysInfoOp.setMachineId(badMachineId);

  BOOST_CHECK_THROW(getSystemInfo(sessionKey, listSysInfo, sysInfoOp), VishnuException);
}

BOOST_AUTO_TEST_SUITE_END()
// THE END
