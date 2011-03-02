/**s
* \file CommandServer.hpp
* \brief This file presents the Class which manipulates Commands on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include <string>
#include <iostream>
#include "SessionServer.hpp"
#include "utilVishnu.hpp"

/**
* \enum CmdType
* \brief The type of the command
*/
typedef enum {
  UMS = 0,/*!< For UMS command type */
  TMS = 1,/*!< For TMS command type */
  FMS = 2,/*!< For FMS command type */
  IMS = 3/*!< For IMS command type */
} CmdType;

/**
* \class CommandServer
* \brief CommandServer class implementation
*/
class CommandServer {
public:
  /**
  * \brief Constructor
  * \fn CommandServer(SessionServer session)
  * \param session The object which encapsulates session data
  */
  CommandServer(SessionServer session);
  /**
  * \brief Constructor
  * \fn CommandServer(std::string cmd, SessionServer session)
  * \param cmd The cmd launched by the user
  * \param session The object which encapsulates session data
  */
	CommandServer(std::string cmd, SessionServer session);
  /**
  * \brief Function to get the command description
  * \fn    std::string getCommand()
  * \return the current command
  */
  std::string
  getCommand();
  /**
  * \brief Function to record the command on the database
  * \fn int record(CmdType cmdType, std::string startTime = "CURRENT_TIMESTAMP", std::string endTime = "CURRENT_TIMESTAMP")
  * \param cmdType The type of the command (UMS, TMS, FMS, IMS)
  * \param startTime The start time of command
  * \param endTime The end time of command
  * \return raises an exception on error
  */
	int
	record(CmdType cmdType,
         std::string startTime = "CURRENT_TIMESTAMP",
         std::string endTime = "CURRENT_TIMESTAMP");
  /**
  * \brief Function to check if commands are running
  * \fn    bool isRunning()
  * \return true if commands are running else false
  */
  bool
  isRunning();
  /**
  * \brief Destructor
  * \fn    ~CommandServer()
  */
	~CommandServer();

  private:
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;
  /**
  * \brief An object which encapsulates session data
  */
  SessionServer msessionServer;
  /**
  * \brief The command launched by the user
  */
  std::string mcommand;
};
#endif
