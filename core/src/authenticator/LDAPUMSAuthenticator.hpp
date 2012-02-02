/**
 * \file LDAPUMSAuthenticator.hpp
 * \brief This file defines the LDAPUMS authenticator object
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#ifndef _LDAPUMSAUTHENTICATOR_H_
#define _LDAPUMSAUTHENTICATOR_H_

#include <iostream>
#include "Authenticator.hpp"
using namespace std;

/**
 * \class LDAPUMSAuthenticator
 * \brief LDAPUMSAuthenticator
 */
class LDAPUMSAuthenticator : public Authenticator {

public :
  /**
   * \brief Constructor
   * \fn LDAPUMSAuthenticator()
   */
  LDAPUMSAuthenticator();
  /**
   * \brief Destructor
   * \fn ~LDAPUMSAuthenticator()
   */
  ~LDAPUMSAuthenticator();
   /**
  * \brief To authenticate a useer
  * \param user The user to authenticate
  * \return true if the corresponding user is
  * authenticated else false
  */
  bool
  authenticate(UMS_Data::User& user);
};


#endif // _LDAPUMSAUTHENTICATOR_H_

