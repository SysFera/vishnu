/**
* \file UserServer.hpp
* \brief This file presents the Class which manipulates VISHNU user data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef USER_SERVER_H
#define USER_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include "VishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "POSTGREDatabase.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "SessionServer.hpp"
#include "utilServer.hpp"
#include "Vishnuid.hpp"

using namespace utilServer;
class SessionServer;

/**
* \class UserServer
* \brief Implementation of the UserServer class
*/
class UserServer {
public:
  /**
  * \brief Constructor
  * \fn UserServer(std::string userId, std::string password) 
  * \param userId The userId of the user
  * \param password The password of the user
  */
  UserServer(std::string userId, std::string password);
  /**
  * \brief Constructor
  * \fn UserServer(const UMS_Data::User& user) 
  * \param user The user data structure
  */
  UserServer(const UMS_Data::User& user);
  /**
  * \brief Constructor
  * \fn UserServer(SessionServer sessionServer) 
  * \param sessionServer The object to manipulate session
  */
  UserServer(SessionServer sessionServer);
  /**
  * \brief Function to add a new VISHNU user
  * \fn int add(UMS_Data::User*& user) 
  * \param user The user data structure
  * \return raises an exception on error
  */
  int 
  add(UMS_Data::User*& user);
  /**
  * \brief Function to update user information
  * \fn int update(UMS_Data::User*& user) 
  * \param user The user data structure
  * \return raises an exception on error
  */
  int 
  update(UMS_Data::User* user);
  /**
  * \brief Function to delete VISHNU user
  * \fn int deleteUser(UMS_Data::User user)
  * \param user The user data structure
  * \return raises an exception on error
  */
  int 
  deleteUser(UMS_Data::User user);
  /**
  * \brief Function to change VISHNU user password
  * \fn int changePassword(std::string newPassword)
  * \param newPassword The new password of the user
  * \return raises an exception on error
  */
  int 
  changePassword(std::string newPassword);
  /**
  * \brief Function to change VISHNU user password
  * \fn int resetPassword(UMS_Data::User user)
  * \param user The user data structure
  * \return raises an exception on error
  */
  int 
  resetPassword(UMS_Data::User user);
  /**
  * \fn ~UserServer()
  * \brief Destructor
  */
  ~UserServer();
  /**
  * \brief Function to get user information
  * \fn UMS_Data::User getData()
  * \return  The user data structure
  */
  UMS_Data::User 
  getData();
  /**
  * \brief Function to initialize user data for constructor with sessionServer 
  * \fn init()
  */
  void 
  init();
  /**
  * \brief Function to check user on database
  * \fn bool exist(bool flagForChangePwd)
  * \param flagForChangePwd A flag to check the password state
  * \return true if the password state has not to be checked else false
  */
  bool 
  exist(bool flagForChangePwd = false);
  /**
  * \brief Function to check the VISHNU user privilege
  * \fn bool isAdmin()
  * \return true if the user is an admin else false
  */
  bool 
  isAdmin();
  /**
  * \brief Function to check the user attribut value
  * \fn bool isAttributOk(std::string attributName, int valueOk)
  * \param attributName The name of the attribut to check
  * \param valueOk the value which will be compare to attribut name value
  * \return true if the attributName value is valueOk
  */
  bool 
  isAttributOk(std::string attributName, int valueOk);//return if the password state ok
  /**
  * \brief Function to get user information from the database vishnu
  * \fn getAttribut(std::string condition, std::string attrname);
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \return the value of the attribut or empty string if no results
  */
  std::string 
  getAttribut(std::string condition, std::string attrname = "numuserid");
  /**
  * \brief Function to check a userId
  * \fn bool existuserId(std::string userId)
  * \param userId The userId to check
  * \return true if the userId exists
  */
  bool
  existuserId(std::string userId);
  
private:
  
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The user data structure
  */
  UMS_Data::User muser;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer *msessionServer;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to generate a password
  * \fn generatePassword(std::string value1, std::string value2)
  * \param value1 a string used to generate the password
  * \param value2 a string used to generate the password
  * \return an encrypted message
  */
  std::string 
  generatePassword(std::string value1, std::string value2);
	
};
#endif
