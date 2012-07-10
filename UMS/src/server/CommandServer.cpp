/**
* \file CommandServer.hpp
* \brief This file implements the Class which manipulates Commands on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "CommandServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"


/**
* \brief Constructor
* \fn CommandServer(SessionServer session)
* \param session The object which encapsulates session data
*/
CommandServer::CommandServer(SessionServer session):msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  mcommand = "";
}

/**
* \brief Constructor
* \fn CommandServer(std::string cmd, SessionServer session)
* \param cmd The cmd launched by the user
* \param session The object which encapsulates session data
*/
CommandServer::CommandServer(std::string cmd, SessionServer session):
  msessionServer(session), mcommand(cmd) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to get the command description
* \fn    std::string getCommand()
* \return the current command
*/
std::string
CommandServer::getCommand() {
  return mcommand;
}

/**
* \brief Function to record the command on the database
* \param cmdType The type of the command (UMS, TMS, FMS, IMS)
* \param cmdStatus The status of the command
* \param newVishnuObjectID the new vishnu object Id
* \param startTime The start time of command
* \param endTime The end time of command
* \return raises an exception on error
*/
int
CommandServer::record(CmdType cmdType,
                      CmdStatus cmdStatus,
                      std::string newVishnuObjectID,
                      std::string startTime,
                      std::string endTime) {

  std::string sqlCmd = std::string("insert into command (vsession_numsessionid, starttime,"
  "endtime, description, ctype, status, vishnuobjectid) values (");

  sqlCmd.append(msessionServer.getAttribut("WHERE "
  "sessionkey='"+msessionServer.getData().getSessionKey()+"'", "numsessionid"));

  sqlCmd.append(","+startTime+ ","+endTime+", '"+mcommand+"',"+convertToString(cmdType)+","+
                convertToString(cmdStatus)+", '"+newVishnuObjectID+"'"+ ")");

  mdatabaseVishnu->process(sqlCmd.c_str());
  return 0;
}

/**
* \brief Function to check if commands are running
* \fn    bool isRunning()
* \return true if commands are running else false
*/
bool
CommandServer::isRunning() {

  std::string sqlCommand("SELECT numcommandid FROM command where endtime is NULL "
  "and vsession_numsessionid=");

  sqlCommand.append(msessionServer.getAttribut("WHERE "
  "sessionkey='"+msessionServer.getData().getSessionKey()+"'", "numsessionid"));
#ifndef USE_SOCI_ADVANCED
  SOCISession session = mdatabaseVishnu->getSingleSession();
  session<<sqlCommand;
  bool got_data=session.got_data();
  mdatabaseVishnu->releaseSingleSession(session);
  return(got_data);
#else
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return (result->getNbTuples() != 0);
#endif
}

/**
* \brief Destructor
* \fn    ~CommandServer()
*/
CommandServer::~CommandServer() {
}


