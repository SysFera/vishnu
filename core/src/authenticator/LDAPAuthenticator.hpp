/**
 * \file LDAPAuthenticator.hpp
 * \brief This file defines the LDAP authenticator object
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#ifndef _LDAPAUTHENTICATOR_H_
#define _LDAPAUTHENTICATOR_H_

#include <iostream>
#include "Authenticator.hpp"
using namespace std;

/**
 * \class LDAPAuthenticator
 * \brief LDAPAuthenticator
 */
class LDAPAuthenticator : public Authenticator {

public :
  /**
   * \brief Constructor
   * \fn LDAPAuthenticator()
   */
  LDAPAuthenticator();
  /**
   * \brief Destructor
   * \fn ~LDAPAuthenticator()
   */
  ~LDAPAuthenticator();
   /**
  * \brief To authenticate a useer
  * \param user The user to authenticate
  * \return true if the corresponding user is
  * authenticated else false
  */
  bool
  authenticate(UMS_Data::User& user);
};


#endif // _LDAPAUTHENTICATOR_H_

