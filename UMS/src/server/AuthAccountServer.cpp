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
* \fn AuthAccountServer(AuthAccount account, SessionServer session)
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
* \fn int add()
* \return raises an exception on error
*/
int
AuthAccountServer::add() {
  std::string sqlCommand = "insert into authaccount (authsystem_authsystemid, users_numuserid, aclogin) values ('%1%', '%2%', '%3%')";
  return addOrUpdate(sqlCommand, false);
}

/**
* \brief Function to update a VISHNU authAccount
* \fn int update()
* \return raises an exception on error
*/
int
AuthAccountServer::update() {
  std::string sqlCommand = "UPDATE authaccount SET aclogin='%3%' where authsystem_authsystemid=%1% and users_numuserid=%2%;";
  return addOrUpdate(sqlCommand, true);
}

/**
 * \ brief Function to add a new account or update an existing one
 * \param sql the sql command to create or update the account.
 *            This string must contain 3 boost format parameters:
 *            authsystem_authsystemid=%1%, users_numuserid=%2% and aclogin=%3%
 * \param update if true then this is an update, false creates a new account
 * \return raises an exception on error
 */
int
AuthAccountServer::addOrUpdate(const std::string &sql, bool update) {
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
    if (authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'"
                                     " and status=1").size() != 0) {

      //To get the database number id of the machine
      numAuthSystem =authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'");
      //To get the database number id of the user
      numUser = userServer.getAttribut("where userid='"+mauthAccount->getUserId()+"'");

      // If we update we need the account to exist
      // if we don't update we need the account not to exist
      if ((!update && !exist(numAuthSystem, numUser))
          || (update && exist(numAuthSystem, numUser))) {
        //check if the acLogin is not already used
        if (isLoginUsed(numAuthSystem, mauthAccount->getAcLogin())) {
          throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
        }

        /* If we create a new account, then we need to execute the sql command
         * thus sqlCommand needs to be not empty.
         * If we update, then we need the AcLogin to be not empty.
         */
        std::string sqlCommand = "";
        if (!update
            || (update && mauthAccount->getAcLogin().size() != 0)) {
          sqlCommand = sql;
        }

        //If there is a change
        if (!sqlCommand.empty()) {
          boost::format(sqlCommand) % numAuthSystem % numUser % mauthAccount->getAcLogin();
          mdatabaseVishnu->process(sqlCommand);
        }
      }//END if the authentification account exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_ACCOUNT);
        throw e;
      }
    } //End if the user-authentication system exists and it is not locked
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_SYSTEM);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}


/**
* \brief Function to delete a VISHNU authAccount
* \fn int deleteAuthAccount()
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
    if (authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'"
      " and status=1").size() != 0) {

      //To get the database number id of the machine
      numAuthSystem = authSystemServer.getAttribut("where authsystemid='"+mauthAccount->getAuthSystemId()+"'");
      //To get the database number id of the user
      numUser = userServer.getAttribut("where userid='"+mauthAccount->getUserId()+"'");

      //if the authentification account exists
      if (exist(numAuthSystem, numUser)) {

        //To remove the authentification account from the database
        mdatabaseVishnu->process("DELETE FROM authaccount "
        "where authsystem_authsystemid="+numAuthSystem+" and users_numuserid="+numUser);

      }//END if the authentification account exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_ACCOUNT);
        throw e;
      }
    } //END if the machine exists and it is not locked
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_SYSTEM);
      throw e;
    }
  }//End if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}
/**
* \fn ~AuthAccountServer
* \brief Destructor
*/
AuthAccountServer::~AuthAccountServer() {
}
/**
* \brief Function to get authAccount information
* \fn UMS_Data::AuthAccount getData()
* \return  The AuthAccount data structure
*/
UMS_Data::AuthAccount *
AuthAccountServer::getData() {
  return mauthAccount;
}
/**
* \brief Function to get authAccount information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
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
  return (getAttribut("where authsystem_authsystemid="+idAuthSystem+" and users_numuserid="+idUser).size() != 0);

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
  std::string numUser = getAttribut("where authsystem_authsystemid="+numAuthSystem+" and aclogin='"+acLogin+"'", "users_numuserid");
  return !numUser.empty();
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
