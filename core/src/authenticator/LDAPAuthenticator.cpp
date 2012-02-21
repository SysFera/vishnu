/**
 * \file LDAPAuthenticator.cpp
 * \brief This file implements the LDAPauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "LDAPAuthenticator.hpp"
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"
#include <openssl/sha.h>
#include "ldap/LDAPProxy.hpp"
#include "DatabaseResult.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "UMSVishnuException.hpp"
#include "SystemException.hpp"


using namespace std;
using namespace boost::archive::iterators;

typedef
    base64_from_binary<
        transform_width<string::const_iterator, 6, 8>
> base64_t;

typedef
    transform_width<
        binary_from_base64<string::const_iterator>, 8, 6
> binary_t;


bool
checkPassword(string sshaLdapPassword, string clearPassword) {
  unsigned char hash[20];
  string s = sshaLdapPassword.substr(6);

  string decodeds(
        binary_t(s.begin()),
        binary_t(s.begin() + s.length())
        );

  string salt = decodeds.substr(20);
  string digest = decodeds.substr(0,20);

  string  newStr = clearPassword;
  newStr.append(salt);

  SHA1((unsigned char*)newStr.c_str(), newStr.size(), hash);

  return (digest.compare(string(reinterpret_cast< char const * >(hash))) == 0);
}


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
                         authlogin,
                         "",
                         authpassword);
      ldapPoxy.connectLDAP(ldapbase);
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

