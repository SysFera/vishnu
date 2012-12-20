/**
 * FMS_fixtures.hpp
 *
 * Author : bisnard
 */
#include "diet_fixtures.hpp"
#include "UMS_fixtures.hpp"
#include "FMS_testconfig.h"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_fms.hpp"
using namespace std;
using namespace vishnu;

char FMSSeD[] = "fmssed";
char ConfigFMSSeD[] = FMSSEDCONF;
char BinDirFMSSeD[] = FMSSEDBINDIR;

class FMSFixture : public UMSSeDFixture {

public:
  FMSFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./fms_automTest";
    // Client configuration file
    string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
    mav[1]= (char*) dietClientConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
    string sqlPath = FMSSQLPATH;
    if (restore(sqlPath + "/cleanall.sql") != 0) {
      cout << "Clean database failed" << endl;
      return;
    }
    if (restore(sqlPath + "/FMSinitTest.sql")!=0) {
      cout << "Database initialization failed" << endl;
      return;
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );

    if (FMSHOST1 == "localhost" ||  FMSHOST2 == "localhost") {
      BOOST_TEST_MESSAGE( "== WARNING: TEST_FMS_HOST1/2 should not be 'localhost' ==" );
    }

  }

  ~FMSFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Finalizing client ==" );
    vishnu::vishnuFinalize();
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Finalizing client ==" );
  }

//private:
  int mac;
  char* mav[2];
};

typedef DietSeDFixture<FMSSeD, BinDirFMSSeD, ConfigFMSSeD, FMSFixture> FMSSeDFixture;
