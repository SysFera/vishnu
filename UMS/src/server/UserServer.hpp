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
   * \brief Function to get user information from the database vishnu
   * \param userid The user id
   * \param attribute the attribute to get
   * \return the value of the attribut or empty string if no results
   */
  std::string
  getEntryAttribute(const std::string& userid, const std::string& attribute);

  /**
   * \brief Check if a couple userid/password is valid
   * \param userid The user id
   * \param password the user password to get
   * \return Return the num user if found. Empty otherwise
   */
  std::string
  getNumUserFromLoginInfo(const std::string& userid, const std::string& password);

  /**
   * @brief Get the DB user identifier
   * @param userid The user id
   * @return
   */
  std::string
  getNumUserFromId(const std::string& userid);

  /**
   * \brief Function to get a user attribute from a session key
   * \param attr The attribute name
   * \param sessionKey The session key
   * \return the value of the attribut or empty string if no results
   */
  std::string
  getAttributeFromSession(const std::string& attr, const std::string& sessionKey);

  /**
   * \brief Function to check a userId
   * \param userId The userId to check
   * \return throw exception if user not found
   */
  void
  checkUserId(const std::string& userId);

  /**
   * \brief Function to get the user account login
   * \param machineId The machine identifier of machine on which the user have a account
   * \return the user account login
   */
  std::string
  getUserAccountLogin(const std::string& machineId);


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
  Database *mdatabase;
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


  /**
   * @brief Save the given user object into database
   * @param user The user object
   */
  void
  dbSave(const UMS_Data::User& user);

};
#endif
