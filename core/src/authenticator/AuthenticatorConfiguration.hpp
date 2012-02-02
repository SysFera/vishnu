/**
 * \file AuthenticatorConfiguration.hpp
 * \brief This file contains the declaration of the AuthenticatorConfiguration class
 * \author Eugène PAMBA CAPO-CHICHI
 * \date 3 Janvier 2012
*/

#ifndef _AUTHENTICATORCONFIGURATION_HPP_
#define _AUTHENTICATORCONFIGURATION_HPP_

#include "ExecConfiguration.hpp"

/**
 * \class AuthenticatorConfiguration
 * \brief Contains the configuration parameters for the Vishnu authenticator
 */
class AuthenticatorConfiguration {

public:

  /**
   * \brief The authenticator type
   */
  typedef enum {
    UMS,
    LDAP,
    UMSLDAP,
    LDAPUMS
  } authentype_t;

  /**
   * \brief Constructor
   * \param execConfig  the configuration of the program
   */
  AuthenticatorConfiguration(const ExecConfiguration& execConfig);

  /**
   * \brief Check that the configuration is correct
   */
  void
  check() throw (UserException);

  /**
   * \brief Get the authenticator type
   * \return authenticator type
   */
  authentype_t
  getAuthenType() const { return mauthType; }


protected:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
   * \brief Reference to the main program configuration
   */
  const ExecConfiguration& mexecConfig;

  /**
   * \brief Attribute type of authenticator
   */
  authentype_t mauthType;
};

#endif // _AUTHENTICATORCONFIGURATION_HPP_
