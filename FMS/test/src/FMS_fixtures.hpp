/**
 * FMS_fixtures.hpp
 *
 * Author : bisnard
 */

#ifndef _FMS_FIXTURES_HPP_
#define _FMS_FIXTURES_HPP_


#include "UMS_fixtures.hpp"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_fms.hpp"
using namespace std;
using namespace vishnu;


class FMSSeDFixture : public UMSSeDFixture {

public:
  FMSSeDFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./fms_automTest";
    // Client configuration file
    string vishnuClientTestConfigPath = getenv("VISHNU_CLIENT_TEST_CONFIG_FILE");
    BOOST_REQUIRE( vishnuClientTestConfigPath.size() !=0  );
    mav[1]= (char*)vishnuClientTestConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==" );
    std::string vishnuTestSetupPath2 = getenv("VISHNU_TEST_SETUP_FILE");
    BOOST_REQUIRE(vishnuTestSetupPath2.size() !=0  );
    FileParser fileparser2(vishnuTestSetupPath2.c_str());
    std::map<std::string, std::string> setupConfig2 = fileparser2.getConfiguration();
    m_test_fms_working_dir = setupConfig2.find("TEST_WORKING_DIR")->second;
    m_test_fms_host1 = setupConfig2.find("TEST_FMS_HOST1")->second;;
    m_test_fms_host2 = setupConfig2.find("TEST_FMS_HOST2")->second;
    m_test_fms_dir1 = setupConfig2.find("TEST_FMS_HOST1_WORKING_DIR")->second;
    m_test_fms_dir2 = setupConfig2.find("TEST_FMS_HOST2_WORKING_DIR")->second;
    m_test_fms_user_login = m_test_ums_user_vishnu_login; 
    m_test_fms_user_pwd = m_test_ums_user_vishnu_pwd;
    m_test_fms_user_local_group =  setupConfig2.find("TEST_FMS_USER_LOCAL_GROUP")->second;


    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );
  }

  ~FMSSeDFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: FMSSeDFixture ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: FMSSeDFixture ==" );
  }

  std::string m_test_fms_working_dir;
  std::string m_test_fms_host1;
  std::string m_test_fms_host2;
  std::string m_test_fms_dir1;
  std::string m_test_fms_dir2;
  std::string m_test_fms_user_login;
  std::string m_test_fms_user_pwd;
  std::string m_test_fms_user_local_group;

  int mac;
  char* mav[2];
};



#endif  // _FMS_FIXTURES_HPP_
