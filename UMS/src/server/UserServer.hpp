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
#include "UMSVishnuException.hpp"
#include "Database.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "SessionServer.hpp"

class SessionServer;

/**
* \class UserServer
* \brief UserServer class implementation
*/
class UserServer {
public:
  /**
   * \brief Constructor
   * \param userId The userId of the user
   * \param password The password of the user
   */
  UserServer(std::string userId, std::string password);
  /**
   * \brief Constructor
   * \param user The user data structure
   */
  UserServer(const UMS_Data::User& user);
  /**
   * \brief Constructor
   * \param sessionServer The object to manipulate session
   */
  UserServer(SessionServer sessionServer);
  /**
   * \brief Function to add a new VISHNU user
   * \param user The user data structure
   * \param sendmailScriptPath The path to the script for sending emails
   * \return raises an exception on error
   */
  int
  add(UMS_Data::User*& user, std::string sendmailScriptPath);
  /**
   * \brief Function to update user information
   * \param user The user data structure
   * \return raises an exception on error
   */
  int
  update(UMS_Data::User* user);
  /**
   * \brief Function to delete VISHNU user
   * \param user The user data structure
   * \return raises an exception on error
   */
  int
  deleteUser(UMS_Data::User user);
  /**
   * \brief Function to change VISHNU user password
   * \param newPassword The new password of the user
   * \return raises an exception on error
   */
  int
  changePassword(const std::string& newPassword);
  /**
   * \brief Function to change VISHNU user password
   * \param user The user data structure
   * \param sendmailScriptPath The path to the script for sending emails
   * \return raises an exception on error
   */
  int
  resetPassword(UMS_Data::User& user, const std::string& sendmailScriptPath);
  /**
   * \brief Destructor
   */
  ~UserServer();
  /**
   * \brief Function to get user information
   * \return  The user data structure
   */
  UMS_Data::User
  getData();
  /**
   * \brief Function to initialize user data for constructor with sessionServer
   */
  void
  init();
  /**
   * \brief Function to check user on database
   * \param flagForChangePwd A flag to check the password state
   * \return true if the password state has not to be checked else false
   */
  bool
  exist(bool flagForChangePwd = false);
  /**
  * \brief Function to authenticate user by using an authentification system
  * \param flagForChangePwd A flag to check the password state
  * \return true if the user is authenticated else false
  */
  bool
  isAuthenticate(bool flagForChangePwd = false);
  /**
   * \brief Function to check the VISHNU user privilege
   * \return true if the user is an admin else false
   */
  bool
  isAdmin();
  /**
   * \brief Function to check the user attribut value
   * \param attributName The name of the attribut to check
   * \param valueOk the value which will be compare to attribut name value
   * \return true if the attributName value is valueOk
   */
  bool
  isAttributOk(std::string attributName, int valueOk);//return if the password state ok
  /**
   * \brief Function to get user information from the database vishnu
   * \param condition The condition of the select request
   * \param attrname the name of the attribut to get
   * \return the value of the attribut or empty string if no results
   */
  std::string
  getAttribut(std::string condition, std::string attrname = "numuserid");
  /**
   * \brief Function to check a userId
   * \param userId The userId to check
   * \return empty or the index of the user in the database
   */
  std::string
  getNumUserId(std::string userId);

   /**
   * \brief Function to get the user account login
   * \param machineId The machine identifier of machine on which the user have a account
   * \return the user account login
   */
  std::string
  getUserAccountLogin(const std::string& machineId);


  /**
  * \brief Function to get a certain user account property
  * \param machineId The machine identifier of machine on which the user have a account
  * \param property The property name
  * \return the user account login
  */
  std::string
  getUserAccountProperty(const std::string& machineId, const std::string& property) ;


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
   * \param value1 a string used to generate the password
   * \param value2 a string used to generate the password
   * \return an encrypted message
   */
  std::string
  generatePassword(std::string value1, std::string value2);

  /**
   * \brief Function to send an email to the user
   * \param user     the user to whom send the email
   * \param content the body of the email
   * \param subject the subject of the email
   * \param sendmailScriptPath The path to the script for sending emails
   * \return The error or success code
   */
  int
  sendMailToUser(const UMS_Data::User& user,
                 const std::string& content,
                 const std::string& subject,
                 const std::string& sendmailScriptPath);

  /**
   * \brief Function to get the email content
   * \param user     the user who will receives the email
   * \param flagAdduser a flag which means that it is uses on adduser function
   * \return the email content
   */
  std::string
  getMailContent(const UMS_Data::User& user, bool flagAdduser);

  /**
   * \brief Function to check the user state (locked or not) and to
   * verify the password state
   * \param flagForChangePwd A flag to check the password state
   */
  void
  CheckUserState(bool flagForChangePwd);

};
#endif
