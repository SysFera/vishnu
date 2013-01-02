/**
 * TMS_FOR_IMS_fixtures.hpp
 *
 * Author : Eugène PAMBA CAPO-CHICHI
 */

#include "UMS_fixtures4ims.hpp"
#include "TMS_testconfig.h"
#include "api_ums.hpp"

using namespace std;
using namespace vishnu;

char TMSSeD[] = "tmssed";
char ConfigTMSSeD[] = TMSSEDCONF;
char BinDirTMSSeD[] = TMSSEDBINDIR;

typedef DietSeDFixture<TMSSeD, BinDirTMSSeD, ConfigTMSSeD, UMSFixture4IMS> TMSSeDForIMSFixture;
