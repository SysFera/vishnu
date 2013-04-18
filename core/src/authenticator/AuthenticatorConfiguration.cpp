/**
 * \file AuthenticatorConfiguration.cpp
 * \brief This file contains the definitions of the AuthenticatorConfiguration class
 * \author Eugène PAMBA CAPO-CHICHI
 * \date 3 Janvier 2012
*/

#include "AuthenticatorConfiguration.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

/**
 * \brief Constructor
 * \param execConfig  the configuration of the program
 */
AuthenticatorConfiguration::AuthenticatorConfiguration(const ExecConfiguration& execConfig) :
mexecConfig(execConfig), mauthType(UMS)
{
}

/**
 * \brief Check that the configuration is correct
 */
void AuthenticatorConfiguration::check() throw (UserException)
{
  string authenTypeStr;
  if (!mexecConfig.getConfigValue(vishnu::AUTHENTYPE, authenTypeStr)) {
    std::cerr << "[INFO][UMS] The default authentication mode has been selected\n";
    authenTypeStr = "UMS";
  }

  std::transform(authenTypeStr.begin(), authenTypeStr.end(), authenTypeStr.begin(), ::tolower);
  if (authenTypeStr == "ums") {
    mauthType = AuthenticatorConfiguration::UMS;
  } else if (authenTypeStr == "ldap") {
    mauthType = AuthenticatorConfiguration::LDAP;
  } else if (authenTypeStr == "umsldap") {
    mauthType = AuthenticatorConfiguration::UMSLDAP;
  } else if (authenTypeStr == "ldapums") {
    mauthType = AuthenticatorConfiguration::LDAPUMS;
  } else {
    throw UserException(ERRCODE_INVALID_PARAM,
    "Invalid authentication mode. Supported mode are: 'UMS', 'LDAP', 'UMSLDAP' or 'LDAPUMS'");
  }
}
