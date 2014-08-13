/**
* \file LocalAccountServer.hpp
* \brief This file presents the Class which manipulates VISHNU localAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef LOCAL_ACCOUNT_SERVER_H
#define LOCAL_ACCOUNT_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "MachineServer.hpp"

/**
* \class LocalAccountServer
* \brief LocalAccountServer class implementation
*/
class LocalAccountServer {
public:
  /**
  * \brief Constructor
  * \param account The LocalAccount data structure
  * \param session The object which encapsulates session data
  */
  LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU localAccount
  * \return raises an exception on error
  */
  int
  add();
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
  deleteLocalAccount();
  /**
   * \brief Destructor,
   */
  ~LocalAccountServer();
  /**
  * \brief Function to get localAccount data structure
  * \return  The LocalAccount data structure
  */
  UMS_Data::LocalAccount*
  getData();

  /**
  * \brief Get the system login from a couple machineid/userid
  * \param machineId The machine ID
  * \param userId The user ID
  * \return The local login, or empty if not exist
  */
  std::string
  getAcLogin(std::string machineId, std::string userId);


  /**
   * @brief Get user database index from the user login and the machine database index
   * @param acLogin The user local login
   * @param numMachine The machine database index
   * @return
   */
  std::string
  getNumUserFromAccount(const std::string&  acLogin, const std::string& numMachine);

  /**
   * @brief Get the database index of the account
   * @param idmachine The database index of the machine
   * @param iduser The database index of the user
   * @return
   */
  std::string
  getNumAccount(const std::string& idmachine, const std::string& iduser);

private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The localAccount data structure
  */
  UMS_Data::LocalAccount *mlocalAccount;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabase;
  /**
   * \brief Mutex to protect the id generation for the mcmd map
   */
  boost::mutex mmutex;

  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
   * \brief Function to check localAccount on database
   * \param idmachine The id of the machine
   * \param iduser The id of the user
   * \return true if the localAccount exists else false
   */
  bool
  existLocalAccount(std::string idmachine, std::string iduser);
  /**
  * \brief Function to check if a given login is used on a machine
  * \param numMachine the internal id of the machine
  * \param acLogin the account login
  * \return true if the login is already used on the machine
  */
  bool
  isLoginUsed(std::string numMachine, std::string acLogin);
};
#endif
