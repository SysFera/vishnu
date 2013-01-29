/**
 * IMS_fixtures.hpp
 *
 * Author : E. PAMBA CAPO-CHICHI
 */

#ifndef _IMS_FIXTURES_HPP_
#define _IMS_FIXTURES_HPP_

#include "TMS_fixtures.hpp"
#include <boost/test/unit_test.hpp>
#include "api_tms.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "FileParser.hpp"
using namespace std;
using namespace vishnu;



class IMSSeDFixture : public TMSSeDFixture {

public:

  std::string m_test_ims_admin_vishnu_login;
  std::string m_test_ims_admin_vishnu_pwd;
  std::string m_test_ims_user_vishnu_login;
  std::string m_test_ims_user_vishnu_pwd;


  IMSSeDFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./ims_automTest";
    // Client configuration file
    string dietClientConfigPath = getenv("VISHNU_CLIENT_TEST_CONFIG_FILE");
    mav[1]= (char*) dietClientConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==" );
    string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    FileParser fileparser(vishnuTestSetupPath.c_str());
    std::map<std::string, std::string> setupConfig = fileparser.getConfiguration();

    m_test_ims_admin_vishnu_login = setupConfig.find("TEST_ADMIN_VISHNU_LOGIN")->second;
    m_test_ims_admin_vishnu_pwd = setupConfig.find("TEST_ADMIN_VISHNU_PWD")->second;
    m_test_ims_user_vishnu_login = setupConfig.find("TEST_USER_VISHNU_LOGIN")->second;
    m_test_ims_user_vishnu_pwd = setupConfig.find("TEST_USER_VISHNU_PWD")->second;
    m_test_ums_user_vishnu_machineid = setupConfig.find("TEST_VISHNU_MACHINEID1")->second;


    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );
  }

  ~IMSSeDFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: IMSSeDFixture ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: IMSSeDFixture ==" );
  }

  int mac;
  char* mav[2];
};


#endif  // _FMS_FIXTURES_HPP_
