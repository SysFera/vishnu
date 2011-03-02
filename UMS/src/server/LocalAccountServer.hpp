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
* \brief Implementation of the LocalAccountServer class
*/
class LocalAccountServer {
public:
  /**
  * \brief Constructor
  * \fn LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session)
  * \param account The LocalAccount data structure
  * \param session The object which encapsulates session data
  */
  LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU localAccount
  * \fn int add() 
  * \return raises an exception on error
  */
  int
  add();
  /**
  * \brief Function to update a VISHNU localAccount
  * \fn int update() 
  * \return raises an exception on error
  */
  int 
  update();
  /**
* \brief Function to delete a VISHNU localAccount
* \fn int deleteLocalAccount() 
* \return raises an exception on error
*/
  int 
  deleteLocalAccount();
  /**
  * \fn ~LocalAccountServer
  * \brief Destructor,
  */
  ~LocalAccountServer();
  /**
  * \brief Function to get localAccount data structure
  * \fn UMS_Data::LocalAccount getData()
  * \return  The LocalAccount data structure
  */
  UMS_Data::LocalAccount* 
  getData();
  /**
  * \brief Function to get localAccount information from the database vishnu
  * \fn getAttribut(std::string condition, std::string attrname);
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \return the value of the attribut or empty string if no results
  */
  std::string 
  getAttribut(std::string condition, std::string attrname = "numaccountid");
  /**
  * \brief Function to check localAccount on database
  * \fn bool exist(std::string idmachine, std::string iduser)
  * \param idmachine The id of the machine
  * \param iduser The id of the user
  * \return true if the localAccount exists else false
  */
  bool 
  exist(std::string idmachine, std::string iduser);
  /**
  * \brief Function to get the content of the public ssh generated
  * \fn std::string getPublicKey()
  * \return The content of the ssh public key generated
  */
  std::string
  getPublicKey();
  
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
  Database *mdatabaseVishnu;
  /**
  * \brief The content of the ssh public key generated
  */
  std::string msshpublickey;
  
  /////////////////////////////////
  // Functions
  /////////////////////////////////
  /**
  * \brief Function to generate a private/public ssh key
  * \fn std::string generateKey()
  */
  std::string 
  generateKey();
};
#endif
