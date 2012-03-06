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
  * \fn AuthSystemServer(UMS_Data::AuthSystem*& account, SessionServer& session)
  * \param authSystem The AuthSystem data structure
  */
  AuthSystemServer(UMS_Data::AuthSystem*& authSystem);
  /**
  * \brief Constructor
  * \fn AuthSystemServer(UMS_Data::AuthSystem*& account, SessionServer& session)
  * \param authSystem The AuthSystem data structure
  * \param session The object which encapsulates session data
  */
  AuthSystemServer(UMS_Data::AuthSystem*& authSystem, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU localAccount
  * \fn int add()
  * \param vishnuId The identifier of the vishnu instance
  * \return raises an exception on error
  */
  int
  add(int vishnuId);
  /**
  * \brief Function to update a VISHNU localAccount
  * \fn int update()
  * \return raises an exception on error
  */
  int
  update();
  /**
* \brief Function to delete a VISHNU localAccount
* \fn int deleteAuthSystem()
* \return raises an exception on error
*/
  int
  deleteAuthSystem();
  /**
  * \fn ~AuthSystemServer
  * \brief Destructor,
  */
  ~AuthSystemServer();
  /**
  * \brief Function to get localAccount data structure
  * \fn UMS_Data::AuthSystem getData()
  * \return  The AuthSystem data structure
  */
  UMS_Data::AuthSystem*
  getData();
  /**
  * \brief Function to get localAccount information from the database vishnu
  * \fn getAttribut(std::string condition, std::string attrname);
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \return the value of the attribut or empty string if no results
  */
  std::string
  getAttribut(std::string condition, std::string attrname = "numauthsystemid");

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
  Database *mdatabaseVishnu;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
 /**
  * \brief Function to check the user-authentication system on database
  * \fn bool exist()
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
};
#endif
