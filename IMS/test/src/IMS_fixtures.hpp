/**
 * IMS_fixtures.hpp
 *
 * Author : E. PAMBA CAPO-CHICHI
 */

#include "diet_fixtures.hpp"
#include "TMS_FOR_IMS_fixtures.hpp"
#include "IMS_testconfig.h"
#include <boost/test/unit_test.hpp>
#include "api_tms.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
using namespace std;
using namespace vishnu;

char IMSSeD[] = "imssed";
char ConfigIMSSeD[] = IMSSEDCONF;
char BinDirIMSSeD[] = IMSSEDBINDIR;

class IMSFixture : public TMSSeDForIMSFixture {

public:
  IMSFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./ims_automTest";
    // Client configuration file
    string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
    mav[1]= (char*) dietClientConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
    string sqlPath = IMSSQLPATH;

    if (restore(sqlPath + "/IMScleanall.sql") != 0) {
      cout << "Clean database failed" << endl;
      return;
    }
    if (restore(sqlPath + "/IMSinitTest.sql")!=0) {
      cout << "Database initialization failed" << endl;
      return;
    }


    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );
  }

  ~IMSFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Finalizing client ==" );
    vishnu::vishnuFinalize();
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Finalizing client ==" );
  }

//private:
  int mac;
  char* mav[2];
};

typedef DietSeDFixture<IMSSeD, BinDirIMSSeD, ConfigIMSSeD, IMSFixture> IMSSeDFixture;
