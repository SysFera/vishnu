/**
* \file MachineServer.hpp
* \brief This file presents the Class which manipulates VISHNU machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef MACHINE_SERVER_H
#define MACHINE_SERVER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <exception>
#include "VishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SessionServer.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"



/**
* \class MachineServer
* \brief MachineServer class implementation
*/
class MachineServer {

public:
  /**
  * \brief Constructor
  * \param machine The machine data structure
  */
  MachineServer(UMS_Data::Machine*& machine);
  /**
  * \brief Constructor
  * \param machine The machine data structure
  * \param session The object which encapsulates session data
  */
  MachineServer(UMS_Data::Machine*& machine, SessionServer& session);
  /**
  * \brief Function to add a new VISHNU machine
  * \return raises an exception on error
  */
  int
  add(void);
  /**
  * \brief Function to update a VISHNU machine
  * \return raises an exception on error
  */
  int
  update();
  /**
  * \brief Function to delete a VISHNU machine
  * \return raises an exception on error
  */
  int
  deleteMachine();
  /**
  * \brief Destructor
  */
  ~MachineServer();
  /**
  * \brief Function to get machine information
  * \return  The user data structure
  */
  UMS_Data::Machine*
  getData();

  /**
  * \brief Get a given machine attribute
  * \param machineId The machine ID
  * \param attribute The attribute
  * \return the value of the attribut or empty string if no results
  */
  std::string
  getEntryAttribute(std::string machineId, std::string attribute);

  /**
   * \brief Function to get the machine
   * \return The name of the machine
   */
  std::string
  getMachineAddress();


  /**
   * @brief Find if a given column a the database have a column with value
   * @param column The column name
   * @param value The value
   * @return
   */
  std::string
  findEntry(const std::string& column, const std::string& value);

  /**
   * @brief Get the nummachine id
   * @param machineId The machine id
   * @return
   */
  std::string
  getNumMachine(const std::string& machineId);

  /**
   * @brief Get the nummachine id
   * @param machineId The machine id
   * @return
   */
  std::string
  getNumActiveMachine(const std::string& machineId);

  /**
   * \brief Function to check the machineId
   * \return raises an exception
   */
  void checkMachine();

private:
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
  * \brief The machine data structure
  */
  UMS_Data::Machine *mmachine;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabase;

  /**
   * @brief Insert the current encapsulatedd object info into database
   * @return
   */
  void
  dbSave(void);
};
#endif
