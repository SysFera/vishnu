/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */


#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "diet_fixtures.hpp"
#include "utils.hpp"
#include "FileParser.hpp"
#include "api_ums.hpp"

namespace ba = boost::assign;
namespace bf = boost::filesystem;
namespace bp = boost::process;
namespace bs = boost::system;

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = "";
char BinDirUMSSeD[] = "";

template <const char *name, const char *binDir, const char *config>
class UMSSEDFixtureTemplate : public DispatcherFixture
{
  
public:
  std::string m_test_ums_authen_type;
  std::string m_test_ums_root_vishnu_login;
  std::string m_test_ums_root_vishnu_pwd;
  std::string m_test_ums_admin_vishnu_login;
  std::string m_test_ums_admin_vishnu_pwd;
  std::string m_test_auth_system_uri;
  std::string m_test_ums_user_vishnu_login;
  std::string m_test_ums_user_vishnu_pwd;
  std::string m_test_ums_user_vishnu_machineid;
  
  UMSSEDFixtureTemplate() {

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    int argc = 2;
    char* argv[argc];
    std::string vishnuClientTestConfigPath = getenv("VISHNU_CLIENT_TEST_CONFIG_FILE");
    BOOST_REQUIRE( vishnuClientTestConfigPath.size() !=0  );
    argv[0]= (char*)"./automTest";
    argv[1]= (char*) vishnuClientTestConfigPath.c_str();
    if (vishnu::vishnuInitialize(argv[1], argc, argv)) {
      BOOST_TEST_MESSAGE( "Error in diet_initialize... (using config: "
      << vishnuClientTestConfigPath << ")" );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );
    
    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );
    std::string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    FileParser fileparser(vishnuTestSetupPath.c_str());
    std::map<std::string, std::string> setupConfig = fileparser.getConfiguration();

    m_test_ums_authen_type = setupConfig.find("TEST_UMS_AUTHEN_TYPE")->second ;
    m_test_ums_root_vishnu_login = setupConfig.find("TEST_ROOT_VISHNU_LOGIN")->second;
    m_test_ums_root_vishnu_pwd = setupConfig.find("TEST_ROOT_VISHNU_PWD")->second;
    m_test_ums_admin_vishnu_login = setupConfig.find("TEST_ADMIN_VISHNU_LOGIN")->second;
    m_test_ums_admin_vishnu_pwd = setupConfig.find("TEST_ADMIN_VISHNU_PWD")->second;
    m_test_auth_system_uri = setupConfig.find("TEST_UMS_AUTH_SYTEM_URI")->second;
    m_test_ums_user_vishnu_login = setupConfig.find("TEST_USER_VISHNU_LOGIN")->second;
    m_test_ums_user_vishnu_pwd = setupConfig.find("TEST_USER_VISHNU_PWD")->second;
    m_test_ums_user_vishnu_machineid = setupConfig.find("TEST_VISHNU_MACHINEID1")->second;

    
    

    
    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==");
  }


  ~UMSSEDFixtureTemplate()
  {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Stopping "
                        << name << " ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Stopping "
                        << name << " ==" );
  }
};

typedef UMSSEDFixtureTemplate<UMSSeD, BinDirUMSSeD, ConfigUMSSeD>  UMSSeDFixture;
