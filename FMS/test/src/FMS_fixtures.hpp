/**
 * FMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "UMS_fixtures.hpp"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_fms.hpp"
using namespace std;
using namespace vishnu;

/*char FMSSeD[] = "fmssed";
char ConfigFMSSeD[] = FMSSEDCONF;
char BinDirFMSSeD[] = FMSSEDBINDIR;*/

class FMSFixture : public UMSSeDFixture {

public:
  FMSFixture():mac(2){

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
    string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    FileParser fileparser(vishnuTestSetupPath.c_str());
    std::map<std::string, std::string> setupConfig = fileparser.getConfiguration();
    m_test_fms_working_dir = setupConfig.find("TEST_WORKING_DIR")->second;
    m_test_fms_host1 = setupConfig.find("TEST_FMS_HOST1")->second;;
    m_test_fms_host2 = setupConfig.find("TEST_FMS_HOST2")->second;
    m_test_fms_dir1 = setupConfig.find("TEST_FMS_HOST1_WORKING_DIR")->second;
    m_test_fms_dir2 = setupConfig.find("TEST_FMS_HOST2_WORKING_DIR")->second;
    m_test_fms_user_login = setupConfig.find("TEST_USER_LOGIN")->second;
    m_test_fms_user_pwd = setupConfig.find("TEST_USER_PWD")->second;
    m_test_fms_local_dir = setupConfig.find("TEST_FMS_LOCAL_DIR")->second;
    m_test_fms_user_local_group =  setupConfig.find("TEST_FMS_USER_LOCAL_GROUP")->second;
    

    
    
    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );

    /*BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
    string sqlPath = m_test_fms_sql_path;                                            
     if (restore(sqlPath + "/cleanall.sql") != 0) {
       cout << "Clean database failed" << endl;
       return;
     }
     if (restore(sqlPath + "/FMSinitTest.sql")!=0) {
       cout << "Database initialization failed" << endl;
       return;
     }
     BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );*/

    /*if (m_test_fms_host1 == "localhost" ||  m_test_fms_host2 == "localhost") {
      BOOST_TEST_MESSAGE( "== WARNING: TEST_FMS_HOST1/2 should not be 'localhost' ==" );
    }*/

  }

  ~FMSFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Finalizing client ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Finalizing client ==" );
  }

  std::string m_test_fms_working_dir;
  std::string m_test_fms_host1;
  std::string m_test_fms_host2;
  std::string m_test_fms_dir1;
  std::string m_test_fms_dir2;
  std::string m_test_fms_user_login;
  std::string m_test_fms_local_dir;
  std::string m_test_fms_user_pwd;
  std::string m_test_fms_user_local_group;
  
//private:
  int mac;
  char* mav[2];
};

typedef FMSFixture FMSSeDFixture;
