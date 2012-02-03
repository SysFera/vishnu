/**
 * \file AuthenticatorConfiguration.cpp
 * \brief This file contains the definitions of the AuthenticatorConfiguration class
 * \author Eugène PAMBA CAPO-CHICHI
 * \date 3 Janvier 2012
*/

#include "AuthenticatorConfiguration.hpp"
#include <algorithm>
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
  mexecConfig.getRequiredConfigValue<std::string>(vishnu::AUTHENTYPE, authenTypeStr);
  std::transform(authenTypeStr.begin(), authenTypeStr.end(), authenTypeStr.begin(), ::tolower);

  if (authenTypeStr == "ums") {
    mauthType = AuthenticatorConfiguration::UMS;
  } else if (authenTypeStr == "ldap") {
    mauthType = AuthenticatorConfiguration::LDAP;
  } else if (authenTypeStr == "umsandldap") {
    mauthType = AuthenticatorConfiguration::UMSLDAP;
  } else if (authenTypeStr == "ldapandums") {
    mauthType = AuthenticatorConfiguration::LDAPUMS;
  } else {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "Configuration for authentification type is invalid"
                        "(must be 'UMS' or 'LDAP' or 'UMSANDLDAP' or 'LDAPANDUMS')");
  }
}

