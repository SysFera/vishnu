/**
* \file LocalAccountServer.cpp
* \brief This file implements the Class which manipulates VISHNU localAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "LocalAccountServer.hpp"
#include "DbFactory.hpp"
#include <boost/format.hpp>

/**
* \brief Constructor
* \param account The LocalAccount data structure
* \param session The object which encapsulates session data
*/
LocalAccountServer::LocalAccountServer(UMS_Data::LocalAccount*& account, SessionServer& session):
  mlocalAccount(account), msessionServer(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::add() {

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  if (! userServer.exist()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_USER);
  }

  // only admin can add local account for other users
  if (userServer.getData().getUserId() != mlocalAccount->getUserId()
      && ! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  machineServer.checkMachine();
  std::string numMachine = machineServer.getNumMachine(mlocalAccount->getMachineId());
  std::string numUser = userServer.getNumUserFromId(mlocalAccount->getUserId());

  if (numUser.empty()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USERID,
                              boost::str(boost::format("User does not exist: %1%")
                                         % mlocalAccount->getUserId()));
  }

  mmutex.lock();
  if (isLoginUsed(numMachine, mlocalAccount->getAcLogin())) {
    mmutex.unlock();
    throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
  }

  std::string query = boost::str(boost::format("INSERT INTO account (machine_nummachineid, "
                                               "        users_numuserid, aclogin, home, status)"
                                               "VALUES ('%1%', '%2%', '%3%', '%4%', %5%)")
                                 % vishnu::convertToLong(numMachine)
                                 % vishnu::convertToLong(numUser)
                                 % mdatabase->escapeData(mlocalAccount->getAcLogin())
                                 % mdatabase->escapeData(mlocalAccount->getHomeDirectory())
                                 % vishnu::STATUS_ACTIVE);
  try  {
    mdatabase->process(query);
  } catch (...) {
    mmutex.unlock();
    throw;
  }

  mmutex.unlock();
  return 0;
}

/**
* \brief Function to update a VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::update() {

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (userServer.getData().getUserId() != mlocalAccount->getUserId()
      && ! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_SESSIONKEY_NOT_FOUND);
  }

  // Check if if the machine exists and is not locked
  if (machineServer.getEntryAttribute(mlocalAccount->getMachineId(), "numachineid").empty()) {
    throw UMSVishnuException (ERRCODE_UNUSABLE_MACHINE);
  }

  // Get the database number id of the machine
  std::string numMachine = machineServer.getEntryAttribute(mlocalAccount->getMachineId(), "numachineid");
  std::string numUser = userServer.getNumUserFromId(mlocalAccount->getUserId());

  mmutex.lock();
  if (! existLocalAccount(numMachine, numUser)) {
    mmutex.unlock();
    throw UMSVishnuException (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
  }
  std::string fields = "";
  if (! mlocalAccount->getAcLogin().empty()) {
    std::string curField = boost::str(boost::format("aclogin='%1%'")
                                      % mdatabase->escapeData(mlocalAccount->getAcLogin()));
    fields += (fields.empty())? curField : ","+curField;
  }


  //if a new home directory has been defined
  if (! mlocalAccount->getHomeDirectory().empty()) {
    std::string curField = (boost::format("home='%1%'")%mdatabase->escapeData(mlocalAccount->getHomeDirectory())).str();
    fields += (fields.empty())? curField : ","+curField;
  }

  //If there is a change
  if (! fields.empty()) {
    std::string sql = boost::str(boost::format("UPDATE account "
                                               " SET %1%"
                                               " WHERE machine_nummachineid=%2%"
                                               "   AND users_numuserid=%3%")
                                 % fields
                                 % numMachine
                                 % numUser);
    mdatabase->process(sql);
  }

  mmutex.unlock();

  return 0;
}

/**
* \brief Function to delete a VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::deleteLocalAccount() {

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  //if the session key is for the owner of the local account or the user is an admin
  if (userServer.getData().getUserId() != mlocalAccount->getUserId()
      && userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  //if the machine exists and it is not locked
  std::string numMachine = machineServer.getNumActiveMachine(mlocalAccount->getMachineId());
  if (numMachine.empty()) {
    throw UMSVishnuException(ERRCODE_UNUSABLE_MACHINE);
  }

  std::string numUser = userServer.getNumUserFromId(mlocalAccount->getUserId());
  if (! existLocalAccount(numMachine, numUser)) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
  }

  std::string sql = boost::str(boost::format("UPDATE account"
                                             " SET status=%1%"
                                             " WHERE machine_nummachineid=%2%"
                                             " AND users_numuserid=%3%"
                                             )
                               % vishnu::STATUS_DELETED
                               % numMachine
                               % numUser);
  mdatabase->process(sql);


  return 0;
}
/**
* \brief Destructor
*/
LocalAccountServer::~LocalAccountServer() {
  //delete mlocalAccount;
}
/**
* \brief Function to get localAccount information
* \return  The LocalAccount data structure
*/
UMS_Data::LocalAccount *
LocalAccountServer::getData() {
  return mlocalAccount;
}

/**
* \brief Get the system login from a couple machineid/userid
* \param machineId The machine ID
* \param userId The user ID
* \return The local login, or empty if not exist
*/
std::string
LocalAccountServer::getAcLogin(const std::string& machineId, const std::string& userId)
{
  std::string query = boost::str(boost::format("SELECT aclogin"
                                               " FROM account, machine, users"
                                               " WHERE machine.machineid   = '%1%'"
                                               "  AND users.userid         = '%2%'"
                                               "  AND account.status       = %3%"
                                               "  AND machine.nummachineid =account.machine_nummachineid"
                                               "  AND users.numuserid      = account.users_numuserid;")
                                 % mdatabase->escapeData(machineId)
                                 % mdatabase->escapeData(userId)
                                 % vishnu::STATUS_ACTIVE);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * @brief Get user database index from the user login and the machine database index
 * @param acLogin The user local login
 * @param numMachine The machine database index
 * @return
 */
std::string
LocalAccountServer::getNumUserFromAccount(const std::string& acLogin, const std::string& numMachine)
{
  std::string query = boost::str(boost::format("SELECT numaccountid "
                                               " FROM account "
                                               "  WHERE machine_nummachineid = %1%"
                                               "   AND aclogin='%2%'"
                                               "   AND status = %3%")
                                 % vishnu::convertToLong(numMachine)
                                 % mdatabase->escapeData(acLogin)
                                 % vishnu::STATUS_ACTIVE);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * @brief Get the database index of the account
 * @param numMachine The database index of the machine
 * @param numUser The database index of the user
 * @return
 */
std::string
LocalAccountServer::getNumAccount(const std::string& numMachine, const std::string& numUser)
{
  std::string query = boost::str(boost::format("SELECT numaccountid"
                                               " FROM account"
                                               " WHERE machine_nummachineid = %1%"
                                               "  AND users_numuserid=%2%"
                                               "  AND status != %3%"
                                               )
                                 % vishnu::convertToLong(numMachine)
                                 % vishnu::convertToLong(numUser)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
* \brief Function to check localAccount on database
* \return true if the localAccount exists else false
*/
bool
LocalAccountServer::existLocalAccount(std::string idmachine, std::string iduser) {

  if (idmachine.empty() || iduser.empty()) {
    return false;
  }
  return ! getNumAccount(idmachine, iduser).empty();
}

/**
 * \brief Function to check if a given login is used on a machine
 * \param numMachine the internal id of the machine
 * \param acLogin the account login
 * \return true if the login is already used
 */
bool
LocalAccountServer::isLoginUsed(std::string numMachine, std::string acLogin) {

  if (numMachine.empty() || acLogin.empty()) {
    return false;
  }
  return ! getNumUserFromAccount(acLogin, numMachine).empty();
}
