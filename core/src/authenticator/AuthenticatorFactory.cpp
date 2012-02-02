/**
 * \file AuthenticatorFactory.cpp
 * \brief This file implements the authenticator factory
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include "AuthenticatorFactory.hpp"
#include "UMSAuthenticator.hpp"
#include "SystemException.hpp"

#ifdef USE_LDAP
#include "LDAPAuthenticator.hpp"
#include "UMSLDAPAuthenticator.hpp"
#include "LDAPUMSAuthenticator.hpp"
#endif

Authenticator* AuthenticatorFactory::mauth = NULL; //%RELAX<MISRA_0_1_3> Used in this file

AuthenticatorFactory::AuthenticatorFactory(){
}

AuthenticatorFactory::~AuthenticatorFactory(){
}

Authenticator*
AuthenticatorFactory::createAuthenticatorInstance(AuthenticatorConfiguration AuthenticatorConfig)
{
  if (mauth != NULL) {
    throw SystemException(ERRCODE_AUTHENTERR, "Authenticator instance already initialized");
  }
  switch (AuthenticatorConfig.getAuthenType()){
    case AuthenticatorConfiguration::UMS :
        mauth = new UMSAuthenticator();
      break;
    case AuthenticatorConfiguration::LDAP :
      #ifdef USE_LDAP
        mauth = new LDAPAuthenticator();
      #else
        throw SystemException(ERRCODE_AUTHENTERR, "LDAP is not enabled (re-compile with VISHNU_USE_LDAP)");
      #endif
      break;
    case AuthenticatorConfiguration::UMSLDAP :
      #ifdef USE_LDAP
        mauth = new UMSLDAPAuthenticator();
      #else
        throw SystemException(ERRCODE_AUTHENTERR, "LDAP is not enabled (re-compile with VISHNU_USE_LDAP)");
      #endif
      break;
    case AuthenticatorConfiguration::LDAPUMS :
      #ifdef USE_LDAP
        mauth = new LDAPUMSAuthenticator();
      #else
        throw SystemException(ERRCODE_AUTHENTERR, "LDAP is not enabled (re-compile with VISHNU_USE_LDAP)");
      #endif
      break;
    default:
    throw SystemException(ERRCODE_AUTHENTERR, "Authenticator instance type unknown or not managed");
  }
  return mauth;
}

Authenticator*
AuthenticatorFactory::getAuthenticatorInstance()
{
  if (mauth == NULL) {
    throw SystemException(ERRCODE_AUTHENTERR, "Authenticator instance not initialized");
  }
  return mauth;
}

