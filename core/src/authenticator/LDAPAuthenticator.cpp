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
  LDAPMessage *searchResult;

  LDAPProxy ldapPoxy("ldap://127.0.0.1:389/",
                      "cn=ldapadmin,dc=edf,dc=fr",
                      "",
                      "secret");

  ldapPoxy.connectLDAP();
  ldapPoxy.searchLDAP("dc=edf,dc=fr",
                      string("uid=").append(user.getUserId()),
                      &searchResult);

  if (ldapPoxy.hasResults(&searchResult)) {
      if (checkPassword(ldapPoxy.getUserPassword(&searchResult),user.getPassword())) {
        authenticated = true;
    }
  }
  ldap_msgfree(searchResult);

  return authenticated;
}

