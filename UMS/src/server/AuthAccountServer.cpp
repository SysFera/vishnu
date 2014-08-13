/**
* \file AuthAccountServer.cpp
* \brief This file implements the Class which manipulates VISHNU authAccount data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "AuthAccountServer.hpp"

#include <boost/format.hpp>
#include <string>

#include "AuthSystemServer.hpp"
#include "DbFactory.hpp"

/**
 * \brief Constructor
 * \param account The AuthAccount data structure
 * \param session The object which encapsulates session data
 */
AuthAccountServer::AuthAccountServer(UMS_Data::AuthAccount*& account, SessionServer& session):
  mauthAccount(account), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to add a new VISHNU authAccount
 * \return raises an exception on error
 */
int
AuthAccountServer::add() {
  return addOrUpdate(false);
}

/**
 * \brief Function to update a VISHNU authAccount
 * \return raises an exception on error
 */
int
AuthAccountServer::update() {
  return addOrUpdate(true);
}

/**
 * \brief Function to add a new account or update an existing one
 * \param update if true then this is an update, false creates a new account
 * \return raises an exception on error
 */
int
AuthAccountServer::addOrUpdate(bool update)
{
  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object authSystem
  UMS_Data::AuthSystem *authSystem = new UMS_Data::AuthSystem();
  authSystem->setAuthSystemId(mauthAccount->getAuthSystemId());
  AuthSystemServer authSystemServer(authSystem);

  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  checkAuthAccountUserId(userServer);

  if (authSystemServer.getNumAuthSystem(mauthAccount->getAuthSystemId()).empty()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM);
  }

  std::string numAuthSystem = authSystemServer.getNumAuthSystem(mauthAccount->getAuthSystemId());
  std::string numUser = userServer.getNumUserFromId(mauthAccount->getUserId());

  if (update && ! exist(numAuthSystem, numUser)) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_AUTH_ACCOUNT);
  }

  if (! update && exist(numAuthSystem, numUser)) {
    throw UMSVishnuException (ERRCODE_AUTH_SYSTEM_ALREADY_EXIST);
  }

  if (isLoginUsed(numAuthSystem, mauthAccount->getAcLogin())) {
    throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
  }

  if (update) {
    std::string query = boost::str(
                          boost::format(
                            "UPDATE authaccount"
                            " SET aclogin='%1%' "
                            " WHERE authsystem_authsystemid=%2%"
                            "  AND users_numuserid=%3%;"
                            )
                          % mdatabaseVishnu->escapeData(mauthAccount->getAcLogin())
                          % numAuthSystem
                          % numUser);

    mdatabaseVishnu->process(query);

  } else if (!mauthAccount->getAcLogin().empty()) {
    std::string query = boost::str(
                          boost::format(
                            "INSERT INTO authaccount (authsystem_authsystemid, users_numuserid, aclogin, status)"
                            " VALUES ('%1%', '%2%', '%3%', %4%);")
                          % numAuthSystem
                          % numUser
                          % mdatabaseVishnu->escapeData(mauthAccount->getAcLogin())
                          % vishnu::STATUS_ACTIVE);

    mdatabaseVishnu->process(query);
  }

  return 0;
}


/**
 * \brief Function to delete a VISHNU authAccount
 * \return raises an exception on error
 */
int
AuthAccountServer::deleteAuthAccount()
{

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object authSystem
  UMS_Data::AuthSystem *authSystem = new UMS_Data::AuthSystem();
  authSystem->setAuthSystemId(mauthAccount->getAuthSystemId());
  AuthSystemServer authSystemServer(authSystem);


  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  checkAuthAccountUserId(userServer);

  if (authSystemServer.getNumAuthSystem(mauthAccount->getAuthSystemId()).empty()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_SYSTEM);
  }

  //To get the database number id of the machine
  std::string numAuthSystem = authSystemServer.getNumAuthSystem(mauthAccount->getAuthSystemId());
  std::string numUser = userServer.getNumUserFromId(mauthAccount->getUserId());

  if (! exist(numAuthSystem, numUser)) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_ACCOUNT);
  }

  std::string query = boost::str(
                        boost::format(
                          "UPDATE authaccount"
                          " SET status=%1%"
                          " WHERE authsystem_authsystemid=%2%"
                          " AND users_numuserid=%3%;"
                          )
                        % vishnu::STATUS_DELETED
                        % numAuthSystem
                        % numUser);

  mdatabaseVishnu->process(query);

  return 0;
}
/**
 * \brief Destructor
 */
AuthAccountServer::~AuthAccountServer() {
}
/**
 * \brief Function to get authAccount information
 * \return  The AuthAccount data structure
 */
UMS_Data::AuthAccount *
AuthAccountServer::getData() {
  return mauthAccount;
}

/**
* \brief Function to check if a given login is used on a user-authentication system
* \param numAuthSystem the internal id of the user-authentication system
* \param numUser the User database identifier
* \return true if the user identifier exists in the database
*/
bool
AuthAccountServer::exist(std::string numAuthSystem, std::string numUser)
{
  if (numAuthSystem.empty() || numUser.empty()) {
    return false;
  }

  std::string query = boost::str(
                        boost::format("SELECT authaccountid"
                                      " FROM authaccount"
                                      " WHERE authsystem_authsystemid=%1%"
                                      "  AND users_numuserid='%2%'"
                                      "  AND status != %3%;")
                        % mdatabaseVishnu->escapeData(numAuthSystem)
                        % mdatabaseVishnu->escapeData(numUser)
                        % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(query));

  return ! (result->getFirstElement().empty());
}


/**
* \brief Function to check if a given login is used on a user-authentication system
* \param numAuthSystem the internal id of the user-authentication system
* \param acLogin the account login
* \return true if the login is already used on the machine
*/
bool
AuthAccountServer::isLoginUsed(std::string numAuthSystem, std::string acLogin) {

  if (numAuthSystem.empty() || acLogin.empty()) {
    return false;
  }

  std::string query = boost::str(
                        boost::format("SELECT users_numuserid"
                                      " FROM authaccount"
                                      " WHERE authsystem_authsystemid=%1%"
                                      "  AND aclogin='%2%'"
                                      "  AND status=%3%;")
                        % mdatabaseVishnu->escapeData(numAuthSystem)
                        % mdatabaseVishnu->escapeData(acLogin)
                        % vishnu::STATUS_ACTIVE);

  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(query));

  return ! (result->getFirstElement().empty());
}

/**
* \brief Function to check the authAccount userId
* \return raises an exception on error
*/
void
AuthAccountServer::checkAuthAccountUserId(UserServer& userServer) {

  if (mauthAccount->getUserId().empty()) {
    mauthAccount->setUserId(userServer.getData().getUserId());
  }

  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }
}
