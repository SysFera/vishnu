/**
* \file AuthAccountServer.hpp
* \brief This file presents the Class which manipulates VISHNU authAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef AUTH_ACCOUNT_SERVER_H
#define AUTH_ACCOUNT_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"

/**
* \class AuthAccountServer
* \brief AuthAccountServer class implementation
*/
class AuthAccountServer {
public:
  /**
   * \brief Constructor
   * \param account The AuthAccount data structure
   * \param session The object which encapsulates session data
   */
  AuthAccountServer(UMS_Data::AuthAccount*& account, SessionServer& session);
  /**
   * \brief Function to add a new VISHNU authAccount
   * \return raises an exception on error
   */
  int
  add();
  /**
   * \brief Function to update a VISHNU authAccount
   * \return raises an exception on error
   */
  int
  update();
  /**
   * \brief Function to delete a VISHNU authAccount
   * \return raises an exception on error
   */
  int
  deleteAuthAccount();
  /**
   * \brief Destructor,
   */
  ~AuthAccountServer();
  /**
   * \brief Function to get authAccount data structure
   * \return  The AuthAccount data structure
   */
  UMS_Data::AuthAccount*
  getData();


private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The authAccount data structure
  */
  UMS_Data::AuthAccount *mauthAccount;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;


  /**
  * \brief Function to check if a given login is used on a user-authentication system
  * \param numAuthSystem the internal id of the user-authentication system
  * \param numUser the User database identifier
  * \return true if the user identifier exists in the database
  */
  bool
  exist(std::string numAuthSystem, std::string numUser);


   /**
  * \brief Function to check if a given login is used on a user-authentication system
  * \param numAuthSystem the internal id of the user-authentication system
  * \param acLogin the account login
  * \return true if the login is already used on the machine
  */
  bool
  isLoginUsed(std::string numAuthSystem, std::string acLogin);
  /**
  * \brief Function to check the authAccount userId
  * \param userServer The user server to use
  * \return raises an exception on error
  */
  void
  checkAuthAccountUserId(UserServer& userServer);

  /**
   * \brief Function to add a new account or update an existing one
   * \param update if true then this is an update, false creates a new account
   * \return raises an exception on error
   */
  int
  addOrUpdate(bool update);
};
#endif
