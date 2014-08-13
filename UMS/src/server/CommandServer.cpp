/**
* \file CommandServer.hpp
* \brief This file implements the Class which manipulates Commands on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "CommandServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include <boost/format.hpp>
using namespace vishnu;

/**
* \brief Constructor
* \param session The object which encapsulates session data
*/
CommandServer::CommandServer(SessionServer session):msessionServer(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mcommand = "";
}

/**
* \brief Constructor
* \param cmd The cmd launched by the user
* \param session The object which encapsulates session data
*/
CommandServer::CommandServer(std::string cmd, SessionServer session):
  msessionServer(session), mcommand(cmd) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Function to get the command description
* \return the current command
*/
std::string
CommandServer::getCommand() {
  return mcommand;
}

/**
* \brief Function to record the command on the database
* \param cmdType The type of the command (UMS, TMS, FMS)
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

  std::string numSession = msessionServer.getAttributFromSessionKey(msessionServer.getData().getSessionKey(), "numsessionid");
  std::string sqlCmd = boost::str(boost::format("INSERT INTO command (vsession_numsessionid, starttime,"
                                                "   endtime, description, ctype, status, vishnuobjectid)"
                                                " VALUES (%1%,%2%,%3%,'%4%',%5%,%6%,'%7%');"
                                                )
                                  % numSession
                                  % startTime
                                  % endTime
                                  % mdatabase->escapeData(mcommand)
                                  % convertToString(cmdType)
                                  % convertToString(cmdStatus)
                                  % mdatabase->escapeData(newVishnuObjectID) );

  return mdatabase->process(sqlCmd).first;
}

/**
* \brief Function to check if commands are running
* \return true if commands are running else false
*/
bool
CommandServer::isRunning() {

  std::string sqlCommand("SELECT numcommandid FROM command where endtime is NULL "
                         "and vsession_numsessionid=");

  sqlCommand.append(msessionServer.getAttributFromSessionKey(msessionServer.getData().getSessionKey(), "numsessionid"));

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(sqlCommand));
  return (result->getNbTuples() != 0);
}

/**
* \brief Destructor
*/
CommandServer::~CommandServer() {
}
