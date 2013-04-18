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
AuthAccountServer::addOrUpdate(bool update) {
  std::string numAuthSystem;
  std::string numUser;

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object authSystem
  UMS_Data::AuthSystem *authSystem = new UMS_Data::AuthSystem();
  authSystem->setAuthSystemId(mauthAccount->getAuthSystemId());
  AuthSystemServer authSystemServer(authSystem);

  //if the user exists
  if (userServer.exist()) {
    //To check the authAccount userId
    checkAuthAccountUserId(userServer);

    //if the user-authentication system exists and it is not locked
    std::string sqlcond = (boost::format("WHERE authsystemid='%1%'"
                                         " AND status=%2%"
                                         )%mauthAccount->getAuthSystemId() %vishnu::STATUS_ACTIVE).str();
    if (!authSystemServer.getAttribut(sqlcond).empty()) {
      //To get the database number id of the machine
      numAuthSystem =authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'");
      //To get the database number id of the user
      numUser = userServer.getAttribut("where userid='"+mauthAccount->getUserId()+"'");

      // Check input data
      if ((!update && !exist(numAuthSystem, numUser)) || (update && exist(numAuthSystem, numUser))) {

        if (isLoginUsed(numAuthSystem, mauthAccount->getAcLogin())) {
          throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
        }
        std::string sql = ("");
        if (update) {
          sql = (boost::format("UPDATE authaccount "
                               " SET aclogin='%1%'"
                               " WHERE authsystem_authsystemid=%2%"
                               " AND users_numuserid=%3%"
                               )
                 %mauthAccount->getAcLogin()
                 %numAuthSystem
                 %numUser
                 ).str();
        } else if (!mauthAccount->getAcLogin().empty()) {
          sql = (boost::format("INSERT INTO authaccount (authsystem_authsystemid, users_numuserid, aclogin, status) "
                               "VALUES ('%1%', '%2%', '%3%', %4%)"
                               )
                 %numAuthSystem
                 %numUser
                 %mauthAccount->getAcLogin()
                 %vishnu::STATUS_ACTIVE).str();
        }
        mdatabaseVishnu->process(sql);
      } else { //END if the authentification account exists
        throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_ACCOUNT);
      }
    } else { //End if the user-authentication system exists and it is not locked
      throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_SYSTEM);
    }
  } else { //End if the user exists
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  return 0;
}


/**
 * \brief Function to delete a VISHNU authAccount
 * \return raises an exception on error
 */
int
AuthAccountServer::deleteAuthAccount() {

  std::string numAuthSystem;
  std::string numUser;

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object authSystem
  UMS_Data::AuthSystem *authSystem = new UMS_Data::AuthSystem();
  authSystem->setAuthSystemId(mauthAccount->getAuthSystemId());
  AuthSystemServer authSystemServer(authSystem);

  //if the user exists
  if (userServer.exist()) {
    //To check the authAccount userId
    checkAuthAccountUserId(userServer);

    //if the machine exists and it is not locked
    std::string sqlcond = (boost::format("WHERE authsystemid='%1%'"
                                         " AND status=%2%"
                                         )%mauthAccount->getAuthSystemId() %vishnu::STATUS_ACTIVE).str();
    if (!(authSystemServer.getAttribut(sqlcond, "numauthsystemid").empty())) {
      //To get the database number id of the machine
      numAuthSystem = authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'");
      //To get the database number id of the user
      numUser = userServer.getAttribut("where userid='"+mauthAccount->getUserId()+"'");

      //if the authentification account exists
      if (exist(numAuthSystem, numUser)) {
        // Set status to DELETED instead of deleting the entry in the database
        std::string sql = (boost::format("UPDATE authaccount"
                                         " SET status=%1%"
                                         " WHERE authsystem_authsystemid=%2%"
                                         " AND users_numuserid=%3%"
                                         )%vishnu::STATUS_DELETED %numAuthSystem %numUser).str();
        mdatabaseVishnu->process(sql);
      } else { //END if the authentification account exists
        throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_ACCOUNT);
      }
    } else { //END if the machine exists and it is not locked
      throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_SYSTEM);
    }
  } else { //End if the user exists
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
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
 * \brief Function to get authAccount information from the database vishnu
 * \param condition The condition of the select request
 * \param attrname the name of the attribut to get
 * \return the value of the attribut or empty string if no results
 */
std::string
AuthAccountServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM authaccount "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to check authAccount on database
* \param idmachine The internal database id of the user-authentication system
* \param iduser The internal database id of the user
* \return true if the authAccount exists else false
*/
bool
AuthAccountServer::exist(std::string idAuthSystem, std::string idUser) {
  if (idAuthSystem.empty() || idUser.empty()) {
    return false;
  }
  std::string sqlcond = (boost::format("WHERE authsystem_authsystemid=%1%"
                                       " AND users_numuserid = %2%"
                                       " AND status != %3%"
                                       )%idAuthSystem %idUser %vishnu::STATUS_DELETED).str();

  return !(getAttribut(sqlcond, "authaccountid").empty());

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
  std::string sqlcond = (boost::format("WHERE authsystem_authsystemid=%1%"
                                       " AND aclogin = '%2%'"
                                       " AND status = %3%"
                                       )%numAuthSystem %acLogin %vishnu::STATUS_ACTIVE).str();

  return !(getAttribut(sqlcond, "users_numuserid").empty());
}

/**
* \brief Function to check the authAccount userId
* \return raises an exception on error
*/
int
AuthAccountServer::checkAuthAccountUserId(UserServer& userServer) {
  //If the authAccount is defined
  if (!mauthAccount->getUserId().empty()) {
    //If the user is not an administrator
    if (!userServer.isAdmin()) {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End If the authAccount is defined
  else {
    mauthAccount->setUserId(userServer.getData().getUserId());
  }

  return 0;
}
