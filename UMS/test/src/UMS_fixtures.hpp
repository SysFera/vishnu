/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "fixtures.hpp"

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = UMSSEDCONF;
//typedef DietSeDFixture<SimpleAddSeD, ConfigSimpleAddSeD> SimpleAddSeDFixture;
typedef DietSeDFixture<UMSSeD, ConfigUMSSeD> UMSSeDFixture;
