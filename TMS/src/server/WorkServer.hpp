/**
* \file WorkServer.hpp
* \brief This file presents the Class which manipulates VISHNU work data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef WORK_SERVER_H
#define WORK_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include "VishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include "Database.hpp"
#include "utilServer.hpp"


/**
* \class WorkServer
* \brief WorkServer class implementation
*/
class WorkServer {

public:
  /**
  * \brief Constructor
  * \param sessionKey The session key
  * \param machineId The machine id
  * \param work The work data structure
  */
  WorkServer(const std::string& sessionKey,
             const std::string& machineId,
             TMS_Data::Work* work);
  /**
  * \brief Function to add a new VISHNU work
  * \param vishnuId The identifier of the vishnu instance
  * \param op the options of the work
  * \return raises an exception on error
  */
  int
  add(int vishnuId, TMS_Data::AddWorkOptions*& op);
  /**
  * \brief Function to update a VISHNU work
  * \return raises an exception on error
  */
  int
  update();
  /**
  * \brief Function to delete a VISHNU work
  * \return raises an exception on error
  */
  int
  deleteWork();
  /**
  * \brief Destructor
  */
  ~WorkServer();
  /**
  * \brief Function to get work information
  * \return  The user data structure
  */
  TMS_Data::Work*
  getData();

  /**
  * \brief Function to get work information from the database vishnu
  * \param condition The condition of the select request
  * \param attrname the name of the attribut to get
  * \return the value of the attribut or empty string if no results
  */
  std::string
  getAttribut(std::string condition, std::string attrname = "id");

  /**
   * \brief Function to get the work
   * \return The name of the work
   */
  std::string getWorkName();

  /**
   * \brief Function to check the workId
   * \return raises an exception
   */
  void checkWork();

private:
  /**
  * \brief The session key
  */
  std::string msessionKey;

  /**
  * \brief The machine Id
  */
  std::string mmachineId;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;

  /**
  * \brief The work data structure
  */
  TMS_Data::Work *mwork;

  /**
  * \brief The options data structure
  */
  TMS_Data::AddWorkOptions *mworkop;

  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseInstance;

  /**
  * \brief The content of the ssh public key
  */
  std::string msshpublickey;

};
#endif
