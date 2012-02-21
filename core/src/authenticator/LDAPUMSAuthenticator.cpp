/**
 * \file LDAPUMSAuthenticator.cpp
 * \brief This file implements the LDAPUMSauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include "LDAPUMSAuthenticator.hpp"
#include "LDAPAuthenticator.hpp"
#include "UMSAuthenticator.hpp"
#include "UMSVishnuException.hpp"
#include "SystemException.hpp"

LDAPUMSAuthenticator::LDAPUMSAuthenticator(){
}

LDAPUMSAuthenticator::~LDAPUMSAuthenticator(){
}

bool
LDAPUMSAuthenticator::authenticate(UMS_Data::User& user) {
  bool authenticated = false;

  UMSAuthenticator umsAuthenticator;
  LDAPAuthenticator ldapAuthenticator;
  SystemException excep;

  //To avoid to return an exception when the first authenticator failed
  try {
    authenticated = ldapAuthenticator.authenticate(user);
  } catch (UMSVishnuException& e) {
    //Do nothing
  } catch (SystemException& e) {
    //Do nothing
    excep.setMsgComp(e.getMsgComp());
    excep.setType(e.getTypeI());
    excep.setMsg(e.getMsgI());
  }

  if (authenticated) {
    return authenticated;
  }
  else {
     authenticated = umsAuthenticator.authenticate(user);
    //if the user is not authenticated
    if (!authenticated) {
      //If an exception has been found
      if (excep.getTypeS().compare("NONE")!=0) {
        throw excep;
      }
    }
    return authenticated;
  }
}

