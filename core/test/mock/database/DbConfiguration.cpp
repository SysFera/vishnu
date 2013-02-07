/**
 * \file DbConfiguration.cpp
 * \brief This file contains the definitions of the DbConfiguration class
 * \author Benjamin Isnard
 * \date April
*/

#include "DbConfiguration.hpp"

using namespace std;

const unsigned DbConfiguration::defaultDbPoolSize = 10;  //%RELAX<MISRA_0_1_3> Used in this file

/**
 * \brief Constructor
 */
DbConfiguration::DbConfiguration() :
    mdbType(MOCK), mdbPort(0),
    mdbPoolSize(defaultDbPoolSize)
{
}

/**
 * \brief Check that the configuration is correct
 */
void DbConfiguration::check()
{
    std::string dbTypeStr;
    mdbType = DbConfiguration::MOCK;
    mdbHost = "localhost";
    mdbPort = 1;
    mdbName = "vishnu";
    mdbUserName = "vishnu_user";
    mdbPassword = "vishnu_user";
    mdbPoolSize = 1;
}
