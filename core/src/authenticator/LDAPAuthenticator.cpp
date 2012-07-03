/**
 * \file LDAPAuthenticator.cpp
 * \brief This file implements the LDAPauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "LDAPAuthenticator.hpp"
#include "ldap/LDAPProxy.hpp"
#include "DatabaseResult.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "UMSVishnuException.hpp"
#include "SystemException.hpp"


LDAPAuthenticator::LDAPAuthenticator(){
}

LDAPAuthenticator::~LDAPAuthenticator(){
}

bool
LDAPAuthenticator::authenticate(UMS_Data::User& user) {
  bool authenticated = false;
  std::string uri, authlogin, authpassword, ldapbase, authSystemStatus, userid, pwd;

  DbFactory factory;
  SOCIDatabase* databaseVishnu = factory.getDatabaseInstance();

  std::string sqlCommand = "SELECT uri, authlogin, authpassword, ldapbase, authsystem.status, userid, pwd"
                           " FROM ldapauthsystem, authsystem, authaccount, users "
                           "where aclogin='"+user.getUserId()+"' and authsystem.authtype="+vishnu::convertToString(LDAPTYPE)
                           +" and authaccount.authsystem_authsystemid=authsystem.numauthsystemid and "
                            "ldapauthsystem.authsystem_authsystemid=authsystem.numauthsystemid and "
                            "authaccount.users_numuserid=users.numuserid";

  boost::scoped_ptr<DatabaseResult> result(databaseVishnu->getResult(sqlCommand.c_str()));

  //If there is no results
  if (result->getNbTuples() == 0) {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER, "There is no user-authentication account declared in VISHNU with this identifier");
    throw e;
  }

  std::vector<std::string> tmp;
  std::vector<std::string>::iterator ii;
  for (int i = 0; i < static_cast <int> (result->getNbTuples()); ++i) {
    tmp.clear();
    tmp = result->get(i);

    ii=tmp.begin();
    uri = *ii;
    authlogin = *(++ii);
    authpassword = *(++ii);
    ldapbase = *(++ii);
    authSystemStatus = *(++ii);
    userid = *(++ii);
    pwd = *(++ii);

    if (vishnu::convertToInt(authSystemStatus) != ACTIVE_STATUS) {
      UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_SYSTEM, "It is locked");
      throw e;
    }

    try {
      LDAPProxy ldapPoxy(uri,
                         user.getUserId(),
                         "",
                         user.getPassword());
      ldapPoxy.connectLDAP(ldapbase);
      authenticated = true;
      user.setUserId(userid);
      user.setPassword(pwd);
      break;
    }
    catch (UMSVishnuException& e) {
      if (e.getMsgI() != ERRCODE_UNKNOWN_USER) {
        throw UMSVishnuException(e);
      }
    }
    catch (SystemException& e) {
      //If there is a connection problem to LDAP and it is not the last LDAP account to check
      if ((e.getMsgI() == ERRCODE_AUTHENTERR) && (i == (static_cast <int> (result->getNbTuples())-1))) {
        throw SystemException(e);
      }
    }
  }
  return authenticated;
}

