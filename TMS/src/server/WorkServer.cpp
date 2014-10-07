/**
* \file WorkServer.cpp
* \brief This file implements the Class which manipulates VISHNU work data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "WorkServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>

/**
* \brief Constructor
* \param sessionKey The session key
* \param machineId The machine id
* \param work The work data structure
*/
WorkServer::WorkServer(const std::string& sessionKey,
                       const std::string& machineId,
                       TMS_Data::Work* work)
  : msessionKey(sessionKey), mmachineId(machineId), mwork(work), mworkop(NULL) {
  DbFactory factory;
  mdatabaseInstance = factory.getDatabaseInstance();

  vishnu::validateAuthKey(msessionKey, machineId, mdatabaseInstance, muserSessionInfo);
}

/**
* \brief Function to add a new VISHNU work
* \return raises an exception on error
*/
int
WorkServer::add(TMS_Data::AddWorkOptions*& mworkop)
{
  if (muserSessionInfo.user_privilege != vishnu::PRIVILEGE_ADMIN) {
    throw TMSVishnuException (ERRCODE_PERMISSION_DENIED, "No sufficient permission");
  }

  mwork->setApplicationId(mworkop->getApplicationId());
  mwork->setSubject(mworkop->getSubject());
  mwork->setOwner(mworkop->getOwner());
  mwork->setDescription(mworkop->getDescription());
  mwork->setProjectId(mworkop->getProjectId());
  mwork->setNbCPU(mworkop->getNbCPU());
  mwork->setStatus(vishnu::STATUS_ACTIVE);


  dbSave();

  return 0;
}

/**
* \brief Destructor
*/
WorkServer::~WorkServer() {
}

/**
* \brief Function to get work information
* \return  The user data structure
*/
TMS_Data::Work*
WorkServer::getData() {
  return mwork;
}

/**
* \brief Function to get work information from the database vishnu
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/

std::string
WorkServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM work "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseInstance->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to get the work
* \return The name of the work
*/
std::string
WorkServer::getWorkName() {

  std::string  workName = getAttribut("where identifier='"+mdatabaseInstance->escapeData(getData()->getWorkId())+"'", "subject");

  return workName;
}

/**
* \brief Function to check the workId
* \return raises an exception
*/
void WorkServer::checkWork() {

  if(getAttribut("where identifier='"+mdatabaseInstance->escapeData(mwork->getWorkId())+"'").size()==0){
    throw TMSVishnuException(ERRCODE_UNKNOWN_WORKID, mwork->getWorkId()+" does not exist among the defined"
                             " work by VISHNU System");
  }
}

/**
 * @brief Insert the current encapsulatedd object info into database
 * @return
 */
void
WorkServer::dbSave(void)
{
  std::string query = boost::str(
                        boost::format("INSERT INTO work"
                                      "(date_created, done_ratio, machine_id, nbcpus, owner_id, status, subject)"
                                      "VALUES('%1%','%2%','%3%','%4%','%5%','%6%','%7%')")
                        % "CURRENT_TIMESTAMP"
                        % mwork->getDoneRatio()
                        % muserSessionInfo.num_machine
                        % mwork->getNbCPU()
                        % muserSessionInfo.num_user
                        % mwork->getStatus()
                        % mwork->getSubject()
                        );


  std::pair<int, uint64_t>
      result =  mdatabaseInstance->process(query);

  mwork->setWorkId( vishnu::convertToString(result.second) );
}
