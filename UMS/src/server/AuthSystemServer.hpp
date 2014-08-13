/**
* \file AuthSystemServer.hpp
* \brief This file presents the Class which manipulates VISHNU AuthSystem data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 03/02/2012
*/

#ifndef AUTH_SYSTEM_SERVER_H
#define AUTH_SYSTEM_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"


/**
* \class AuthSystemServer
* \brief AuthSystemServer class implementation
*/
class AuthSystemServer {
public:
  /**
  * \brief Constructor
  * \param authSystem The AuthSystem data structure
  */
  AuthSystemServer(UMS_Data::AuthSystem*& authSystem);
  /**
  * \brief Constructor
  * \param authSystem The AuthSystem data structure
  * \param session The object which encapsulates session data
  */
  AuthSystemServer(UMS_Data::AuthSystem*& authSystem, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU localAccount
  * \return raises an exception on error
  */
  int
  add(void);
  /**
  * \brief Function to update a VISHNU localAccount
  * \return raises an exception on error
  */
  int
  update();
  /**
   * \brief Function to delete a VISHNU localAccount
   * \return raises an exception on error
   */
  int
  deleteAuthSystem();
  /**
  * \brief Destructor,
  */
  ~AuthSystemServer();
  /**
  * \brief Function to get localAccount data structure
  * \return  The AuthSystem data structure
  */
  UMS_Data::AuthSystem*
  getData();

  /**
  * \brief Get the value of a given attribute of a given NON-DELETED entry
  * \param authSystemID The identifier of the auth system
  * \param attr The attribute name
  * \return the value of the attribut or empty string if no results
  */
  std::string
  getEntryAttribute(const std::string& authSystemID, const std::string& attr);

  /**
  * \brief Get the database ID of an auth system entry from its auth system identifier
  * \param authId The auth system identifier
  * \return the value as string or empty if not found
  */
  std::string
  getNumAuthSystem(const std::string& authId);

private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The AuthSystem data structure
  */
  UMS_Data::AuthSystem *mauthsystem;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabase;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to check the user-authentication system on database
  * \return true if the user-authentication system exists else false
  */
  bool
  exist();
  /**
  * \brief Function to check the user-authentication system parameters values
  *\return raises an exception on error
  */
  int
  checkValues();
  /**
  * \brief Function to check the $USERNAME string on the ldap base
  *\return raises an exception on error
  */
  int
  checkLdapBase();

  /**
   * @brief Insert the current encapsulatedd object info into database
   * @return
   */
  void
  dbSave(void);
};
#endif
