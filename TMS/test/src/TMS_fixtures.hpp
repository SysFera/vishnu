/**
 * TMS_fixtures.hpp
 *
 * Author : bisnard
 */

//#include "UMS_fixtures.hpp"
#include "TMS_testconfig.h"
#include "TMS_fixtures_init.hpp"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"
using namespace std;
using namespace vishnu;

char TMSSeD[] = "tmssed";
char ConfigTMSSeD[] = TMSSEDCONF2;
char BinDirTMSSeD[] = TMSSEDBINDIR;

typedef DietSeDFixture<TMSSeD, BinDirTMSSeD, ConfigTMSSeD, TMSSeDFixtureInit> TMSSeDFixture;
