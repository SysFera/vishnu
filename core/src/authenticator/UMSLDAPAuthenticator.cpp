/**
 * \file UMSLDAPAuthenticator.cpp
 * \brief This file implements the UMSLDAPauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include "UMSLDAPAuthenticator.hpp"
#include "LDAPAuthenticator.hpp"
#include "UMSAuthenticator.hpp"

UMSLDAPAuthenticator::UMSLDAPAuthenticator(){
}

UMSLDAPAuthenticator::~UMSLDAPAuthenticator(){
}

bool
UMSLDAPAuthenticator::authenticate(UMS_Data::User& user) {
  bool authenticated = false;

  UMSAuthenticator umsAuthenticator;
  LDAPAuthenticator ldapAuthenticator;

  //The password changed on the authenticate of UMS that is why the clear version is saved
  string ldapPassword = user.getPassword();

  if (umsAuthenticator.authenticate(user)) {
    authenticated = true;
  }
  else {
    user.setPassword(ldapPassword);
    authenticated = ldapAuthenticator.authenticate(user);
  }
  return authenticated;
}

