/**
 * TMS_fixtures.hpp
 *
 * Author : bisnard
 */
#ifndef _TMS_FIXTURES_HPP_
#define _TMS_FIXTURES_HPP_


#include "FMS_fixtures.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "ExecConfiguration.hpp"



struct Machine_TMS {
  std::string machine_id;
  std::string batch_name;
};

class TMSSeDFixture : public FMSSeDFixture {

public:

  std::string m_test_tms_user_vishnu_login;
  std::string m_test_tms_user_vishnu_pwd;
  std::string m_test_tms_working_dir;

  std::vector<Machine_TMS> m_test_tms_machines;
  TMSSeDFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./tms_automTest";
    // Client configuration file
    std::string vishnuClientTestConfigPath = getenv("VISHNU_CLIENT_TEST_CONFIG_FILE");
    BOOST_REQUIRE( vishnuClientTestConfigPath.size() !=0  );
    mav[1]= (char*)vishnuClientTestConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==" );
    std::string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    std::string key = "TEST_TMS_MACHINE_IDS";
    ExecConfiguration tmsConfiguration;
    tmsConfiguration.initFromFile(vishnuTestSetupPath);
    std::vector<std::string> tmsMachines;
    tmsConfiguration.getConfigValues(key, tmsMachines);
    for(std::vector<std::string>::iterator it = tmsMachines.begin(); it != tmsMachines.end();++it)
    {
      Machine_TMS ma;
      std::vector<std::string> machine;
      boost::split(machine, *it, boost::is_any_of(" "));
      if (machine.size() == 2 )
      {
        ma.machine_id = machine.at(0);
        ma.batch_name = machine.at(1);
        m_test_tms_machines.push_back(ma);
      }
    }
    FileParser fileparser(vishnuTestSetupPath.c_str());
    std::map<std::string, std::string> setupConfig = fileparser.getConfiguration();


    m_test_tms_user_vishnu_login = setupConfig.find("TEST_USER_VISHNU_LOGIN")->second;
    m_test_tms_user_vishnu_pwd = setupConfig.find("TEST_USER_VISHNU_PWD")->second;
    m_test_tms_working_dir = setupConfig.find("TEST_WORKING_DIR")->second;


    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );
  }

  ~TMSSeDFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: TMSSeDFixture ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: TMSSeDFixture ==" );
  }

  int mac;
  char* mav[2];
};

#endif  // _TMS_FIXTURES_HPP_
