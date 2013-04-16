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
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::add() {

  std::string numMachine;
  std::string numUser;

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
        userServer.isAdmin()){
      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
                                    " and status=1").size() != 0) {
        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account does not exist
        if (!exist(numMachine, numUser)) {

          if (!isLoginUsed(numMachine, mlocalAccount->getAcLogin())) {
            //The sql code to insert the localAccount on the database
            std::string sqlCmd = (boost::format("INSERT INTO account (machine_nummachineid, "
                                                  "        users_numuserid, aclogin, sshpathkey, home, status)"
                                                  "VALUES ('%1%', '%2%', '%3%', '%4%', '%5%', %6%)")
                                    %numMachine
                                    %numUser
                                    %mlocalAccount->getAcLogin()
                                    %mlocalAccount->getSshKeyPath()
                                    %mlocalAccount->getHomeDirectory()
                                    %vishnu::STATUS_ACTIVE).str();
            mdatabaseVishnu->process(sqlCmd);
            msshpublickey = machineServer.getAttribut("where "
                                                      "machineid='"+mlocalAccount->getMachineId()+"'", "sshpublickey");
          } else {
            throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
          }
        } else {
          throw UMSVishnuException (ERRCODE_LOCAL_ACCOUNT_EXIST);
        }
      } else {
        throw UMSVishnuException (ERRCODE_UNUSABLE_MACHINE);
      }
    } else {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }
  } else {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  return 0;
}

/**
* \brief Function to update a VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::update() {
  std::string numMachine;
  std::string numUser;
  std::string sqlCommand = "";

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
        userServer.isAdmin()){

      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
                                    " and status=1").size() != 0) {

        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account exists
        if (exist(numMachine, numUser)) {

          //check if the acLogin is not already used
          if (isLoginUsed(numMachine, mlocalAccount->getAcLogin())) {
            throw UMSVishnuException(ERRCODE_LOGIN_ALREADY_USED);
          }

          //if a new acLogin has been defined
          if (mlocalAccount->getAcLogin().size() != 0) {
            sqlCommand.append("UPDATE account SET aclogin='"+mlocalAccount->getAcLogin()+"'"
                              " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //if a new sshpathkey has been defined
          if (mlocalAccount->getSshKeyPath().size() != 0) {
            sqlCommand.append("UPDATE account SET sshpathkey='"+mlocalAccount->getSshKeyPath()+"'"
                              " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //if a new home directory has been defined
          if (mlocalAccount->getHomeDirectory().size() != 0) {
            sqlCommand.append("UPDATE account SET home='"+mlocalAccount->getHomeDirectory()+"'"
                              " where machine_nummachineid="+numMachine+" and users_numuserid="+numUser+";");
          }

          //If there is a change
          if (!sqlCommand.empty()) {
            mdatabaseVishnu->process(sqlCommand.c_str());
          }

        }//END if the local account exists
        else {
          UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
          throw e;
        }
      } //End if the machine exists and it is not locked
      else {
        UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
        throw e;
      }
    }//if the session key is for the owner of the local account or the user is an admin
    else {
      UMSVishnuException e (ERRCODE_SESSIONKEY_NOT_FOUND);
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
* \brief Function to delete a VISHNU localAccount
* \return raises an exception on error
*/
int
LocalAccountServer::deleteLocalAccount() {

  std::string numMachine;
  std::string numUser;


  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //Creation of the object machine
  UMS_Data::Machine *machine = new UMS_Data::Machine();
  machine->setMachineId(mlocalAccount->getMachineId());
  MachineServer machineServer = MachineServer(machine);

  //if the user exists
  if (userServer.exist()) {
    //if the session key is for the owner of the local account or the user is an admin
    if (userServer.getData().getUserId().compare(mlocalAccount->getUserId()) == 0 ||
        userServer.isAdmin()){

      //if the machine exists and it is not locked
      if (machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'"
                                    " and status=1").size() != 0) {

        //To get the database number id of the machine
        numMachine = machineServer.getAttribut("where machineid='"+mlocalAccount->getMachineId()+"'");
        //To get the database number id of the user
        numUser = userServer.getAttribut("where userid='"+mlocalAccount->getUserId()+"'");

        //if the local account exists
        if (exist(numMachine, numUser)) {
          // Set status to DELETED instead of deleting the entry in the database
          std::string sql = (boost::format("UPDATE account"
                                           " SET status=%1%"
                                           " WHERE machine_nummachineid=%2%"
                                           " AND users_numuserid=%3%"
                                           )%vishnu::STATUS_DELETED %numMachine %numUser).str();
          mdatabaseVishnu->process(sql);
        }//END if the local account exists
        else {
          UMSVishnuException e (ERRCODE_UNKNOWN_LOCAL_ACCOUNT);
          throw e;
        }
      } //END if the machine exists and it is not locked
      else {
        UMSVishnuException e (ERRCODE_UNUSABLE_MACHINE);
        throw e;
      }
    }//END if the session key is for the owner of the local account or the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
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
* \brief Function to get localAccount information from the database vishnu
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string
LocalAccountServer::getAttribut(std::string condition, std::string attrname) {

  std::string sqlCommand("SELECT "+attrname+" FROM account "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
}

/**
* \brief Function to check localAccount on database
* \return true if the localAccount exists else false
*/
bool
LocalAccountServer::exist(std::string idmachine, std::string iduser) {

  if (idmachine.empty() || iduser.empty()) {
    return false;
  }
  std::string sqlcond = (boost::format("WHERE machine_nummachineid = %1%"
                                       " AND users_numuserid=%2%"
                                       " AND status != %3%"
                                       )%idmachine %iduser %vishnu::STATUS_DELETED).str();
  return !(getAttribut(sqlcond, "numaccountid").empty());
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
  std::string sqlcond = (boost::format("WHERE machine_nummachineid = %1%"
                                       " AND aclogin='%2%'"
                                       " AND status != %3%"
                                       )%numMachine %acLogin %vishnu::STATUS_DELETED).str();

  std::string numUser = getAttribut(sqlcond, "users_numuserid");
  return !(numUser.empty());
}

/**
* \brief Function to get the content of the public ssh generated
* \return The content of the ssh public key generated
*/
std::string
LocalAccountServer::getPublicKey() {
  return msshpublickey;
}
