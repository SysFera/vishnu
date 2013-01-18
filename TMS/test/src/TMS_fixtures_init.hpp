/**
 * TMS_fixtures.hpp
 *
 * Author : bisnard
 */
#include "diet_fixtures.hpp"
#include "FMS_fixtures.hpp"
#include "TMS_testconfig.h"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"
using namespace std;
using namespace vishnu;

char TMSSeDInit[] = "tmssed";
char ConfigTMSSeDInit[] = TMSSEDCONF;
char BinDirTMSSeDInit[] = TMSSEDBINDIR;

class TMSFixtureInit : public FMSSeDFixture {

public:
  TMSFixtureInit():mac(2){

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

    /*BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
    string sqlPath = TMSSQLPATH;
    if (restore(sqlPath + "/cleanall.sql") != 0) {
      cout << "Clean database failed" << endl;
      return;
    }
    if (restore(sqlPath + "/TMSinitTest.sql")!=0) {
      cout << "Database initialization failed" << endl;
      return;
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );*/

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==" );
    string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    
    
    
    
    BOOST_TEST_MESSAGE( "== Test setup [END]: LOADING SETUP ==" );

  }

  ~TMSFixtureInit() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Finalizing client ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Finalizing client ==" );
  }

//private:
  int mac;
  char* mav[2];
};

typedef TMSFixtureInit TMSSeDFixtureInit;
