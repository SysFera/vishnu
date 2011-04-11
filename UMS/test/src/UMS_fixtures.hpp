/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "vishnu_fixtures.hpp"
#include "testconfig.h"

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = UMSSEDCONF;
char BinDirUMSSeD[] = UMSSEDBINDIR;

typedef VishnuSeDFixture<UMSSeD, BinDirUMSSeD, ConfigUMSSeD, DietMAFixture> UMSSeDFixture;
