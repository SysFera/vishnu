/**
 * TMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "UMS_fixtures.hpp"
#include "TMS_testconfig.h"
//#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"
using namespace std;

char TMSSeD[] = "tmssed";
char ConfigTMSSeD[] = TMSSEDCONF;
char BinDirTMSSeD[] = TMSSEDBINDIR;

typedef DietSeDFixture<TMSSeD, BinDirTMSSeD, ConfigTMSSeD, UMSSeDFixture> TMSSeDFixture;

class TMSFixture : public TMSSeDFixture {

public:
  TMSFixture():mac(2){

    // Name of the test executable
    mav[0]= (char*)"./tms_automTest";
    // Client configuration file
    string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
    mav[1]= (char*) dietClientConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }

    BOOST_TEST_MESSAGE( " Vishnu is initialized..." );

  }

  ~TMSFixture(){

  vishnu::vishnuFinalize();

  BOOST_TEST_MESSAGE( " Vishnu is finalized..." );
}

private:
  int mac;
  char* mav[2];
};
