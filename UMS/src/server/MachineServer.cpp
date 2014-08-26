/**
* \file MachineServer.cpp
* \brief This file implements the Class which manipulates VISHNU machine data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "MachineServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include <boost/format.hpp>
#include "utilServer.hpp"

/**
* \brief Constructor
* \param machine The machine data structure
*/
MachineServer::MachineServer(UMS_Data::Machine*& machine):
  mmachine(machine)
{
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \param machine The machine data structure
* \param session The object which encapsulates session data
*/
MachineServer::MachineServer(UMS_Data::Machine*& machine, SessionServer& session):
  mmachine(machine), msessionServer(session)
{
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU machine
* \return raises an exception on error
*/
int
MachineServer::add(void) {

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  dbSave();

  return 0;
}

/**
* \brief Function to update a VISHNU machine
* \return raises an exception on error
*/
int
MachineServer::update() {

  std::string sqlCommand = "";

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  //if the machine to update exists
  std::string numMachineId = getEntryAttribute(mmachine->getMachineId(), "nummachineid");
  if (numMachineId.empty()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_MACHINE);
  }

  if (! mmachine->getAddress().empty()) {
    std::string query = boost::str(boost::format("UPDATE machine SET address='%1%'"
                                                 " WHERE nummachineid='%2%';")
                                   % mdatabase->escapeData(mmachine->getAddress())
                                   % numMachineId);
    sqlCommand.append(query);
  }

  if (mmachine->getStatus() != vishnu::STATUS_UNDEFINED) {
    std::string query = boost::str(
                          boost::format("UPDATE machine SET status='%1%'"
                                        " WHERE nummachineid='%2%';")
                          % mmachine->getStatus()
                          % numMachineId);
    sqlCommand.append(query);
  }

  if (! mmachine->getDescription().empty()) {
    std::string query = boost::str(
                          boost::format("UPDATE machine SET description='%1%'"
                                        " WHERE machine_nummachineid='%2%';")
                          % mdatabase->escapeData(mmachine->getDescription())
                          % numMachineId);

    sqlCommand.append(query);
  }

  if (! sqlCommand.empty()) {
    mdatabase->process(sqlCommand);
  }

  return 0;
}

/**
* \brief Function to delete a VISHNU machine
* \return raises an exception on error
*/
int
MachineServer::deleteMachine() {
  mmachine->setStatus(vishnu::STATUS_DELETED);
  int ret = update();
  if (ret == 0){

    std::string query = boost::str(boost::format("UPDATE account, machine "
                                                 "  SET account.status='%1%' "
                                                 "  WHERE machine.nummachineid=account.machine_nummachineid "
                                                 "   AND machine.nummachineid='%2%';")
                                   % vishnu::STATUS_DELETED
                                   % mdatabase->escapeData(mmachine->getMachineId()));

    return mdatabase->process(query).first;
  }
  return ret;
}

/**
* \brief Destructor
*/
MachineServer::~MachineServer() {
}

/**
* \brief Function to get machine information
* \return  The user data structure
*/
UMS_Data::Machine*
MachineServer::getData() {
  return mmachine;
}


/**
* \brief Get a given machine attribute
* \param machineId The machine ID
* \param attribute The attribute
* \return the value of the attribut or empty string if no results
*/
std::string
MachineServer::getEntryAttribute(std::string machineId, std::string attribute)
{
  std::string query = boost::str(boost::format("SELECT %1% "
                                               " FROM machine"
                                               " WHERE machineid='%2%'"
                                               "  AND status!='%3%'")
                                 % attribute
                                 % machineId
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
* \brief Function to get the machine
* \return The name of the machine
*/
std::string
MachineServer::getMachineAddress() {

  std::string query = boost::str(boost::format("SELECT address FROM machine WHERE machineid='%1%'")
                                 % mdatabase->escapeData(getData()->getMachineId()));

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * @brief Find if a given column a the database have a column with value
 * @param column The column name
 * @param value The value
 * @return The machine database id or empty if not found
 */
std::string
MachineServer::findEntry(const std::string& column, const std::string& value)
{
  std::string query = boost::str(boost::format("SELECT nummachineid"
                                               " FROM machine "
                                               " WHERE %1% = '%2%'"
                                               "  AND status != %3%;")
                                 % mdatabase->escapeData(column)
                                 % mdatabase->escapeData(value)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * @brief Get the nummachine id
 * @param machineId The machine id
 * @return
 */
std::string
MachineServer::getNumMachine(const std::string& machineId)
{
  std::string query = boost::str(boost::format("SELECT nummachineid"
                                               " FROM machine "
                                               " WHERE machineid = '%1%'"
                                               "   AND status != '%2%'")
                                 % mdatabase->escapeData(machineId)
                                 % vishnu::STATUS_DELETED);
  boost::scoped_ptr<DatabaseResult> result( mdatabase->getResult(query) );
  return result->getFirstElement();
}


/**
 * @brief Get the nummachine id
 * @param machineId The machine id
 * @return
 */
std::string
MachineServer::getNumActiveMachine(const std::string& machineId)
{
  std::string query = boost::str(boost::format("SELECT nummachineid"
                                               " FROM machine "
                                               " WHERE machineid = '%1%'"
                                               "   AND status = '%2%'")
                                 % mdatabase->escapeData(machineId)
                                 % vishnu::STATUS_ACTIVE);
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));
  return result->getFirstElement();
}

/**
* \brief Function to check the machineId
* \return raises an exception
*/
void MachineServer::checkMachine() {

  std::string result = getEntryAttribute(mmachine->getMachineId(), "status");
  if (result.empty()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE,
                             boost::str(boost::format("No machine with this id (%1%)")
                                        % mmachine->getMachineId()));
  }

  if (vishnu::convertToInt(result) == vishnu::STATUS_LOCKED) {
    throw UMSVishnuException(ERRCODE_MACHINE_LOCKED);
  }
}

/**
 * @brief Insert the current encapsulatedd object info into database
 * @return
 */
void
MachineServer::dbSave(void)
{
  std::string query = boost::str(boost::format("INSERT INTO machine (machineid, address, description, status)"
                                               "VALUES ('%1%','%2%','%3%','%4%')")
                                 % mmachine->getMachineId()
                                 % mmachine->getAddress()
                                 % mmachine->getDescription()
                                 % vishnu::STATUS_ACTIVE);
  mdatabase->process(query);
}
