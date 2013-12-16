/**
 * \file LDAPAuthenticator.cpp
 * \brief This file implements the LDAPauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include "LDAPAuthenticator.hpp"

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>

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
  Database* databaseVishnu = factory.getDatabaseInstance();
  std::string sqlCommand = (boost::format("SELECT uri, authlogin, authpassword, ldapbase, authsystem.status, userid, pwd"
                                          " FROM ldapauthsystem, authsystem, authaccount, users"
                                          " WHERE aclogin='%1%'"
                                          " AND authsystem.authtype=%2%"
                                          " AND authaccount.authsystem_authsystemid=authsystem.numauthsystemid"
                                          " AND ldapauthsystem.authsystem_authsystemid=authsystem.numauthsystemid"
                                          " AND authaccount.users_numuserid=users.numuserid"
                                          " AND authsystem.status<>%3%"
                                          " AND users.status<>%4%"
                              )%databaseVishnu->escapeData(user.getUserId()) %LDAPTYPE %vishnu::STATUS_DELETED %vishnu::STATUS_DELETED).str();

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

    if (vishnu::convertToInt(authSystemStatus) != vishnu::STATUS_ACTIVE) {
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
