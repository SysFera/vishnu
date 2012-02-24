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
#include "UMSVishnuException.hpp"
#include "SystemException.hpp"

UMSLDAPAuthenticator::UMSLDAPAuthenticator(){
}

UMSLDAPAuthenticator::~UMSLDAPAuthenticator(){
}

bool
UMSLDAPAuthenticator::authenticate(UMS_Data::User& user) {
  bool authenticated = false;

  UMSAuthenticator umsAuthenticator;
  LDAPAuthenticator ldapAuthenticator;
  SystemException excep;
  UMSVishnuException umsexcep;
  bool umsexcepfound = false;
  bool excepfound = false;

  //The password changed on the authenticate of UMS that is why the clear version is saved
  string ldapPassword = user.getPassword();
  //To avoid to return an exception when the first authenticator failed
  try {
    authenticated = umsAuthenticator.authenticate(user);
  } catch (UMSVishnuException& e) {
    umsexcep = e;
    umsexcepfound = true;
    //Do not throw exception
  } catch (SystemException& e) {
    //Do not throw exception
    excep = e;
    excepfound = true;
  }

  if (authenticated) {
    return authenticated;
  }
  else {
    user.setPassword(ldapPassword);
    authenticated = ldapAuthenticator.authenticate(user);
    //if the user is not authenticated
    if (!authenticated) {
      //If an exception has been found
      if (excepfound) {
        throw excep;
      }
      if (umsexcepfound) {
        throw umsexcep;
      }
    }
    return authenticated;
  }
}

