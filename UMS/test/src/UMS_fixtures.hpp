/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "fixtures.hpp"
#include "testconfig.h"

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = UMSSEDCONF;
char BinDirUMSSeD[] = "/tmp/umssed";
typedef DietSeDFixture<UMSSeD, BinDirUMSSeD, ConfigUMSSeD, DietMAFixture> UMSSeDFixture;
