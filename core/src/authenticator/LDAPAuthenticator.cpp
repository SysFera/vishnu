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
  std::string uri, authlogin, authpassword, ldapbase, userpwdencryption, userid, pwd;

  DbFactory factory;
  Database* databaseVishnu = factory.getDatabaseInstance();

  //TODO: rajouter le status et vérifier le status (authsystem not locked)
  std::string sqlCommand = "SELECT uri, authlogin, authpassword, ldapbase, userpwdencryption, userid, pwd"
                           " FROM ldapauthsystem, authsystem, authaccount, users "
                           "where aclogin='"+user.getUserId()+"' and authsystem.authtype="+vishnu::convertToString(LDAPTYPE)
                           +" and authaccount.authsystem_authsystemid=authsystem.numauthsystemid and "
                            "ldapauthsystem.authsystem_authsystemid=authsystem.numauthsystemid and "
                            "authaccount.users_numuserid=users.numuserid";

  boost::scoped_ptr<DatabaseResult> result(databaseVishnu->getResult(sqlCommand.c_str()));

  //If there is no results
  if (result->getNbTuples() == 0) {
    UMSVishnuException e (ERRCODE_UNKNOWN_USERID, "There is no user-authentication account declared in VISHNU with this identifier");
    throw e;
  }

  std::vector<std::string> tmp;
  std::vector<std::string>::iterator ii;
  for (int i = 0; i < static_cast <int> (result->getNbTuples()); ++i) {
    LDAPMessage *searchResult;
    tmp.clear();
    tmp = result->get(i);

    ii=tmp.begin();
    uri = *ii;
    std::cout << "uri:" << uri << std::endl; ii++;
    authlogin = *ii;
    std::cout << "authlogin:" << authlogin << std::endl; ii++;
    authpassword = *ii;
    std::cout << "authpassword:" << authpassword << std::endl; ii++;
    ldapbase = *ii;
    std::cout << "ldapbase:" << ldapbase << std::endl; ii++;
    userpwdencryption = *ii;
    //TODO: faire une fonction pour vérifier le type d'encryption
    std::cout << "userpwdencryption:" << userpwdencryption << std::endl; ii++;
    userid = *ii;
    std::cout << "userid:" << userid << std::endl; ii++;
    pwd = *ii;
    std::cout << "pwd:" << pwd << std::endl;
    std::cout << std::endl;

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
        throw e;
      }
    }
    catch (SystemException& e) {
      //If there is a connection problem to LDAP and it is not the last LDAP account to check
      if ((e.getMsgI() == ERRCODE_AUTHENTERR) && (i == (static_cast <int> (result->getNbTuples())-1))) {
        throw e;
      }
    }
  }
  return authenticated;
}

