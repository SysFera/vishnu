/**
 * \file AuthenticatorFactory.hpp
 * \brief This file defines the authenticator factory
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#ifndef _AUTHENTICATORFACTORY_H_
#define _AUTHENTICATORFACTORY_H_

#include "Authenticator.hpp"
#include "AuthenticatorConfiguration.hpp"

/**
 * \class AuthenticatorFactory
 * \brief Authenticatorfactory
 */
class AuthenticatorFactory{

public :
  /**
   * \brief Constructor
   * \fn AuthenticatorFactory()
   */
  AuthenticatorFactory();
  /**
   * \brief Destructor
   * \fn ~AuthenticatorFactory()
   */
  ~AuthenticatorFactory();
  /**
   * \brief Function to create an authenticator from a configuration
   * \param AuthenticatorConfig the configuration of the authenticator
   * \return An authenticator or a nil pointer
   */
  Authenticator*
  createAuthenticatorInstance(AuthenticatorConfiguration AuthenticatorConfig);

  /**
   * \brief Get the single instance of the database
   * \return A authenticator or a nil pointer
   */
  Authenticator*
  getAuthenticatorInstance();

private :
  /**
   * \brief The unique instance of the database
   */
  static Authenticator* mauth;
};


#endif // _AUTHENTICATORFACTORY_H_

