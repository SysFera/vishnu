/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "diet_fixtures.hpp"
#include "IMS_testconfig.h"
#include "testconfig.h"

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = UMSSEDCONF;
char BinDirUMSSeD[] = UMSSEDBINDIR;
//char LogConfDir[] = IMSLOGCONF;

typedef DietSeDFixture<UMSSeD, BinDirUMSSeD, ConfigUMSSeD, DietLogServiceAndMAFixture> UMSAndLogServicefixture;
//typedef DietSeDFixture<UMSSeD, BinDirUMSSeD, ConfigUMSSeD, LogServiceFixtureConf> UMSAndLogServicefixture;