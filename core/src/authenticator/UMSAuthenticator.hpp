/**
 * \file UMSAuthenticator.hpp
 * \brief This file defines the UMS authenticator object
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#ifndef _UMSAUTHENTICATOR_H_
#define _UMSAUTHENTICATOR_H_

#include "Authenticator.hpp"

/**
 * \class UMSAuthenticator
 * \brief UMSAuthenticator
 */
class UMSAuthenticator : public Authenticator {

public :
  /**
   * \brief Constructor
   */
  UMSAuthenticator();
  /**
   * \brief Destructor
   */
  ~UMSAuthenticator();
   /**
  * \brief To authenticate a useer
  * \param user The user to authenticate
  * \return true if the corresponding user is
  * authenticated else false
  */
  bool
  authenticate(UMS_Data::User& user);
};


#endif // _UMSAUTHENTICATOR_H_
