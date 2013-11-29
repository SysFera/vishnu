/**
* \file UserServer.cpp
* \brief This file implements the Class which manipulates VISHNU user data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include <boost/scoped_ptr.hpp>
#include "UserServer.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "RequestFactory.hpp"
#include "LocalAccountServer.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "AuthenticatorFactory.hpp"
#include <boost/foreach.hpp>
#include <boost/format.hpp>

using namespace vishnu;


/**
 * \brief Constructor
 * \param userId The userId of the user
 * \param password The password of the user
 */
UserServer::UserServer(std::string userId, std::string password) {
  DbFactory factory;
  muser.setUserId(userId);
  muser.setPassword(password);
  mdatabaseVishnu = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
 * \brief Constructor
 * \param user The user data structure
 */
UserServer::UserServer(const UMS_Data::User& user):muser(user) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
 * \brief Constructor
 * \param sessionServer The object to manipulate session
 */
UserServer::UserServer(SessionServer sessionServer): msessionServer(&sessionServer) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to add a new VISHNU user
 * \param user The user data structure
 * \param vishnuId The identifier of the vishnu instance
 * \param sendmailScriptPath The path to the script for sending emails
 * \return raises an exception on error
 */
int
UserServer::add(UMS_Data::User*& user, int vishnuId, std::string sendmailScriptPath) {
  std::string pwd;
  std::string sqlUpdate = "update users set ";

  std::string idUserGenerated;
  std::string passwordCrypted;

  if (exist()) {
    if (isAdmin()) {

      //Generation of password
      pwd = generatePassword(user->getLastname(), user->getFirstname());
      user->setPassword(pwd.substr(0,PASSWORD_MAX_SIZE));

      //Generation of userid
      idUserGenerated = vishnu::getObjectId(vishnuId,
                                            "formatiduser",
                                            USER,
                                            user->getLastname());

      user->setUserId(idUserGenerated);
      //To get the password encrypted
      passwordCrypted = vishnu::cryptPassword(user->getUserId(), user->getPassword());

      // If there only one field reserved by getObjectId
      std::string sqlcond = (boost::format("WHERE userid = '%1%'"
                                           " AND status != %2%;"
                                           )%user->getUserId()%convertToString(vishnu::STATUS_DELETED)).str();

      if (getAttribut(sqlcond, "count(numuserid)") == "0") {
        //To active the user status
        user->setStatus(vishnu::STATUS_ACTIVE);
        sqlUpdate+="vishnu_vishnuid="+convertToString(vishnuId)+", ";
        sqlUpdate+="pwd='"+mdatabaseVishnu->escapeData(passwordCrypted)+"', ";
        sqlUpdate+="firstname='"+mdatabaseVishnu->escapeData(user->getFirstname())+"', ";
        sqlUpdate+="lastname='"+mdatabaseVishnu->escapeData(user->getLastname())+"', ";
        sqlUpdate+="privilege="+convertToString(user->getPrivilege())+", ";
        sqlUpdate+="email='"+user->getEmail()+"', ";
        sqlUpdate+="passwordstate=0, ";
        sqlUpdate+="status="+convertToString(user->getStatus())+" ";
        sqlUpdate+="where userid='"+user->getUserId()+"';";
        mdatabaseVishnu->process(sqlUpdate);


        //Send email
        std::string emailBody = getMailContent(*user, true);
        sendMailToUser(*user, emailBody, "Vishnu message: user created", sendmailScriptPath);

      }// END If the user to add exists
      else {
        UMSVishnuException e (ERRCODE_USERID_EXISTING);
        throw e;
      }
    } //END if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  } //END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}//END: add(UMS_Data::User*& user)

/**
 * \brief Function to update user information
 * \param user The user data structure
 * \return raises an exception on error
 */
int
UserServer::update(UMS_Data::User *user) {
  std::string sqlquery = "UPDATE users SET ";
  std::string comma="";
  bool updateDataProvided = false;
  bool closeAccount = false;
  if (exist()) {
    if (isAdmin()) {
      //if the user whose information will be updated exists
      if (existuserId(user->getUserId())) {

        //if a new fisrtname has been defined
        if (!user->getFirstname().empty()) {
          sqlquery.append(" firstname='"+mdatabaseVishnu->escapeData(user->getFirstname())+"'");
          comma=",";
          updateDataProvided = true;
        }

        //if a new lastname has been defined
        if (!user->getLastname().empty()) {
          sqlquery.append(comma + " lastname='"+mdatabaseVishnu->escapeData(user->getLastname())+"'");
          comma=",";
          updateDataProvided = true;
        }

        //if a new email has been defined
        if (!user->getEmail().empty()) {
          sqlquery.append(comma+" email='"+mdatabaseVishnu->escapeData(user->getEmail())+"'");
          comma=",";
          updateDataProvided = true;
        }

        //If a new status has been defined
        if (user->getStatus() != vishnu::STATUS_UNDEFINED) {
          //if the user will be locked
          if (user->getStatus() == vishnu::STATUS_LOCKED) {
            sqlquery.append(comma + " status="+convertToString(user->getStatus())+" ");
            comma=",";
            updateDataProvided = true;
            closeAccount = true;

            //if the user is not already locked
            if (convertToInt(getAttribut(" WHERE  userid='"+user->getUserId()+"'", "status")) == vishnu::STATUS_LOCKED) {
              std::cerr << boost::format("[WARNING] User already locked %1%\n")%user->getUserId();
            }
          } else {
            sqlquery.append(comma + " status="+convertToString(user->getStatus())+" ");
            comma=",";
            updateDataProvided = true;
          }
        }
        // Check if the user privileges should be update
        if (user->getPrivilege() != vishnu::PRIVILEGE_UNDEFINED) {
          sqlquery.append(comma+" privilege="+convertToString(user->getPrivilege())+" ");
          comma=",";
          updateDataProvided = true;
        }

        // Process the query if there is changes
        if (updateDataProvided) {
          sqlquery.append((boost::format(" WHERE userid = '%1%'"
                                         " AND status != %2%;"
                                         )%user->getUserId() %convertToString(vishnu::STATUS_DELETED)).str());
          mdatabaseVishnu->process(sqlquery);

          // close the current user sessions if the user account has been locked
          if (closeAccount) {
            sqlquery = (boost::format("UPDATE vsession"
                                      " SET state='%1%'"
                                      " WHERE sessionkey IN (SELECT sessionkey"
                                      "   FROM (SELECT users_numuserid, STATE FROM vsession) AS sessioninfo, users"
                                      "   WHERE sessioninfo.users_numuserid=users.numuserid"
                                      "   AND users.userid='%2%' AND sessioninfo.state='%3%');"
                                      )
                        %convertToString(vishnu::SESSION_CLOSED)
                        %user->getUserId()
                        %convertToString(vishnu::SESSION_ACTIVE)).str();
            mdatabaseVishnu->process(sqlquery);
          }
        }
      } else {
        throw UMSVishnuException (ERRCODE_UNKNOWN_USERID);
      }
    } else {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }
  } else {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  return 0;
} //END: update(UMS_Data::User *user)

/**
 * \brief Function to delete VISHNU user
 * \param user The user data structure
 * \return raises an exception on error
 */
int
UserServer::deleteUser(UMS_Data::User user) {
  user.setStatus(vishnu::STATUS_DELETED);
  int ret = update(&user);
  if (ret == 0){
    std::string req = mdatabaseVishnu->getRequest(VR_UPDATE_ACCOUNT_WITH_USERS);
    std::string sqlUpdate = (boost::format(req)
                             %vishnu::STATUS_DELETED
                             %user.getUserId()
                             ).str();
    return mdatabaseVishnu->process(sqlUpdate.c_str());
  }
  return ret;
}//END: deleteUser(UMS_Data::User user)

/**
 * \brief Function to change VISHNU user password
 * \param newPassword The new password of the user
 * \return raises an exception on error
 */
int
UserServer::changePassword(std::string newPassword) {
  std::string sqlChangePwd;
  std::string sqlUpdatePwdState;
  //the flagForChangePwd is set to true to avoid the password state checking

  //If the user exist
  if (isAuthenticate(true)) {
    //If the identifiers used for the connection are a global VISHNU identifiers registered on UMS database
    if (!getAttribut("where userid='"+muser.getUserId()+"'", "numuserid").empty()) {
      //Encrypt the password with the global userId as a salt
      newPassword = vishnu::cryptPassword(muser.getUserId(), newPassword);

      //sql code to change the user password
      sqlChangePwd = (boost::format("UPDATE users SET pwd='%1%'"
                                    " WHERE userid='%2%' AND pwd='%3%' AND status !=%4%;"
                                    )
                      %newPassword
                      %muser.getUserId()
                      %muser.getPassword()
                      %convertToString(vishnu::STATUS_DELETED)
                      ).str();

      //append SQL query to update the passwordstate
      sqlChangePwd += (boost::format("UPDATE users SET passwordstate=1 "
                                     " WHERE userid='%1%' AND pwd='%2% AND status=%3%;"
                                     )
                       %muser.getUserId()
                       %newPassword
                       %convertToString(vishnu::STATUS_DELETED)).str();

      mdatabaseVishnu->process(sqlChangePwd);

      //Put the new user's password
      muser.setPassword(newPassword);
    } else {
      throw UMSVishnuException (ERRCODE_READONLY_ACCOUNT);
    }
  } else {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  return 0;
}//END: changePassword(std::string newPassword)

/**
 * \brief Function to change VISHNU user password
 * \param user The user data structure
 * \param sendmailScriptPath The path to the script for sending emails
 * \return raises an exception on error
 */
int
UserServer::resetPassword(UMS_Data::User& user, std::string sendmailScriptPath) {
  std::string sqlResetPwd;
  std::string sqlUpdatePwdState;
  std::string passwordCrypted;
  std::string pwd;


  //If the user exists
  if (exist()) {
    //if the user is an admin
    if (isAdmin()) {
      //if the user whose password will be reset exists
      if (getAttribut("where userid='"+user.getUserId()+"' AND status !='"+ convertToString(vishnu::STATUS_DELETED)+"'").size() != 0) {
        //generation of a new password
        pwd = generatePassword(user.getUserId(), user.getUserId());
        user.setPassword(pwd.substr(0,PASSWORD_MAX_SIZE));

        //to get the password encryptes
        passwordCrypted = vishnu::cryptPassword(user.getUserId(), user.getPassword());

        //The sql code to reset the password
        sqlResetPwd = "UPDATE users SET pwd='"+passwordCrypted+"' where "
                      "userid='"+user.getUserId()+"' AND status !='"+ convertToString(vishnu::STATUS_DELETED)+"';";
        //sql code to update the passwordstate
        sqlUpdatePwdState = "UPDATE users SET passwordstate=0 "
                            "where userid='"+user.getUserId()+"' and pwd='"+passwordCrypted+"' AND status !='"+ convertToString(vishnu::STATUS_DELETED)+"';";
        //To append the previous sql codes
        sqlResetPwd.append(sqlUpdatePwdState);
        //Execution of the sql code on the database
        mdatabaseVishnu->process(sqlResetPwd.c_str());
        //to get the email adress of the user
        std::string email = getAttribut("where userid='"+user.getUserId()+"' AND status !='"+convertToString(vishnu::STATUS_DELETED)+"'", "email");
        user.setEmail(email);
        //Send email
        std::string emailBody = getMailContent(user, false);
        sendMailToUser(user, emailBody, "Vishnu message: password reset", sendmailScriptPath);
      } // End if the user whose password will be reset exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USERID, "You must use a global VISHNU identifier");
        throw e;
      }
    } //END if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  } //END if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}//END: resetPassword(UMS_Data::User user)

/**
 * \brief Destructor
 */
UserServer::~UserServer() {
}
/**
 * \brief Function to get user information
 * \return  The user data structure
 */
UMS_Data::User
UserServer::getData() {
  return muser;
}

/**
 * \brief Function to initialize user data for constructor with sessionServer
 */
void
UserServer::init(){
  std::string numUser;
  std::string sessionState;

  //if userId and password have not been defined
  if ((muser.getUserId().size() == 0) && (muser.getPassword().size() == 0)) {
    //To get the users_numuserid by using the sessionServer
    numUser =
        msessionServer->getAttribut("where"
                                    " sessionkey='"+msessionServer->getData().getSessionKey()+"'", "users_numuserid");

    //if the session key is found
    if (numUser.size() != 0) {
      //To get the session state
      sessionState =
          msessionServer->getAttribut("where"
                                      " sessionkey='"+msessionServer->getData().getSessionKey()+"'", "state");

      //if the session is active
      if (convertToInt(sessionState) == vishnu::STATUS_ACTIVE) {
        muser.setUserId(getAttribut("where numuserid='"+numUser+"'", "userid"));
        muser.setPassword(getAttribut("where numuserid='"+numUser+"'", "pwd"));
      } //End if the session is active
      else {
        throw UMSVishnuException (ERRCODE_SESSIONKEY_EXPIRED);
      }

    } //END If the session key is found
    else {
      UMSVishnuException e (ERRCODE_SESSIONKEY_NOT_FOUND);
      throw e;
    }
  }//END If the userId and password have not been defined
} //END: void init()
/**
 * \brief Function to check user on database
 * \param flagForChangePwd A flag to check the password state
 * \return true if the password state has not to be checked else false
 */
bool
UserServer::exist(bool flagForChangePwd) {
  //if the user is on the database

  std::string sqlcond = (boost::format("WHERE userid = '%1%'"
                                       " AND pwd='%2%'"
                                       " AND status != %3%"
                                       )%muser.getUserId() %muser.getPassword() %vishnu::STATUS_DELETED).str();
  if (!getAttribut(sqlcond, "numuserid").empty()) {
    CheckUserState(flagForChangePwd);
    return true;
  }
  return false;
}

/**
* \brief Function to check user on database
* \return true if the user is authenticated else false
*/
bool
UserServer::isAuthenticate(bool flagForChangePwd) {

  bool existUser;
  AuthenticatorFactory authFactory;
  Authenticator *authenticatorInstance = authFactory.getAuthenticatorInstance();

  existUser = authenticatorInstance->authenticate(muser);
  if (existUser) {
    CheckUserState(flagForChangePwd);
  }
  return existUser;
}


/**
 * \brief Function to check the VISHNU user privilege
 * \return true if the user is an admin else false
 */
bool
UserServer::isAdmin() {

  return (convertToInt (getAttribut("where userid='"+muser.getUserId()+"'and "
                                    "pwd='"+muser.getPassword()+"'", " privilege")) != 0);
}

/**
 * \brief Function to check the user attribut value
 * \param attributName The name of the attribut to check
 * \param valueOk the value which will be compare to attribut name value
 * \return true if the attributName value is valueOk
 */
bool
UserServer::isAttributOk(std::string attributName, int valueOk) {
  return (convertToInt(getAttribut("where userid='"+muser.getUserId()+"'and "
                                   "pwd='"+muser.getPassword()+"' AND status !='"+ convertToString(vishnu::STATUS_DELETED)+"'", attributName)) == valueOk);
}

/**
 * \brief Function to get user information from the database vishnu
 * \param condition The condition of the select request
 * \param attrname the name of the attribut to get
 * \return the value of the attribut or empty string if no results
 */
std::string UserServer::getAttribut(std::string condition, std::string attrname) {
  std::string sqlCommand("SELECT "+attrname+" FROM users "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand));
  return result->getFirstElement();
}

/**
 * \brief Function to check a userId
 * \param userId The userId to check
 * \return true if the userId exists
 */
bool
UserServer::existuserId(std::string userId) {

  std::string sqlcond = (boost::format("WHERE userid = '%1%'"
                                       " AND status != %2%")%userId %vishnu::STATUS_DELETED).str();
  if (!getAttribut(sqlcond, "numuserid").empty())	{
    return true;
  }
  return false;
}

/**
 * \brief Function to generate a password
 * \param value1 a string used to generate the password
 * \param value2 a string used to generate the password
 * \return an encrypted message
 */
std::string
UserServer::generatePassword(std::string value1, std::string value2) {

  std::string salt = convertToString(generateNumbers());
  std::string clef = value2+convertToString(generateNumbers());

  return (boost::str(boost::format("%1%%2%") % crypt(clef.c_str(), salt.c_str()) % salt.length()));
}
/**
* \brief Function to send an email to a user
* \param user     the user to whom send the email
* \param content  the body of the email
* \param subject  the subject of the email
* \param sendmailScriptPath The path to the script for sending emails
*/
int
UserServer::sendMailToUser(const UMS_Data::User& user,
                           std::string content,
                           std::string subject,
                           std::string sendmailScriptPath) {

  std::vector<std::string> tokens;
  std::ostringstream command;
  pid_t pid;

  std::string address = user.getEmail();
  //If the address is empty
  if (address.empty()) {
    throw UserException(ERRCODE_INVALID_MAIL_ADRESS, "Empty email address");
  }

  //If the script is empty
  if (sendmailScriptPath.empty()) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid server configuration");
  }
  // To build the script command
  command << sendmailScriptPath << " --to " << address << " -s ";

  std::istringstream is(command.str());
  std::copy(std::istream_iterator<std::string>(is),
            std::istream_iterator<std::string>(),
            std::back_inserter<std::vector<std::string> >(tokens));

  char* argv[tokens.size()+6];
  argv[tokens.size()+5]=NULL;
  //Use of tokens
  for (unsigned int i = 0; i < tokens.size(); ++i) {
    argv[i]=strdup(tokens[i].c_str());
  }
  //To avoid mutiple values by using tokens because of spaces
  argv[tokens.size()]=strdup(subject.c_str());
  argv[tokens.size()+1]=strdup(content.c_str());
  //To execute the script on background
  argv[tokens.size()+2]=strdup(" 1>/dev/null ");
  argv[tokens.size()+3]=strdup(" 2>/dev/null ");
  argv[tokens.size()+4]=strdup(" & ");

  pid = fork();
  if (pid == -1) {//if an error occurs during fork
    for (unsigned int i=0; i<tokens.size()+5; ++i) {
      free(argv[i]);
    }
    throw SystemException(ERRCODE_SYSTEM, "Error during the creation of the process for sending mail to "
                          +user.getFirstname()+ " with userId:" +user.getUserId());
  }

  if (pid == 0) {//if the child process
    freopen("dev/null", "r", stdin);
    freopen("dev/null", "w", stdout);
    freopen("dev/null", "w", stderr);

    if (execv(argv[0], argv) == -1) {
      for (unsigned int i=0; i<tokens.size()+5; ++i) {
        free(argv[i]);
      }
      exit(1);
    }
  }
  for (unsigned int i=0; i<tokens.size()+5; ++i) {
    free(argv[i]);
  }
  return 0;
}

/**
* \brief Function to get the email content
* \param user     the user who will receives the email
* \param flagAdduser a flag which means that it is uses on adduser function
* \return the email content
*/
std::string
UserServer::getMailContent(const UMS_Data::User& user, bool flagAdduser) {
  std::string content;
  std::stringstream newline;
  newline << std::endl;

  if (flagAdduser) {
    content.append("Dear "+user.getFirstname()+" "+user.getLastname()+ ",");
    content.append(newline.str());
    content.append("This is respectively your userId and your password generated by vishnu:");
    content.append(newline.str());
    content.append("userId:"+ user.getUserId()+",");
    content.append(newline.str());
    content.append("password:"+user.getPassword());
  }
  else {
    content.append("Dear "+user.getUserId()+",");
    content.append(newline.str());
    content.append("This is your new password: "+user.getPassword());
  }
  return content;
}

/**
* \brief Function to get the user account login
* \param machineId The machine identifier of machine on which the user have a account
* \return the user account login
*/
std::string
UserServer::getUserAccountLogin(const std::string& machineId) {

  init();

  std::string userId = getData().getUserId();
  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();
  account->setMachineId(machineId);
  account->setUserId(userId);
  LocalAccountServer localAccount(account, *msessionServer);

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  MachineServer machineServer(machine);
  std::string sqlcond("");
  std::string numMachine("");
  std::string numUser("");
  std::string acLogin("");
  sqlcond = (boost::format("WHERE machineid = '%1%'"
                           " AND status = %2%")%localAccount.getData()->getMachineId() %vishnu::STATUS_ACTIVE).str();
  numMachine = machineServer.getAttribut(sqlcond, "nummachineid");

  sqlcond = (boost::format("WHERE userid = '%1%'"
                           " AND status = %2%")%localAccount.getData()->getUserId() %vishnu::STATUS_ACTIVE).str();
  numUser = getAttribut(sqlcond, "numuserid");

  if ((numMachine.size() > 0) && (numUser.size() > 0)) {
    sqlcond = (boost::format("WHERE machine_nummachineid = %1%"
                             " AND users_numuserid=%2%"
                             " AND status = %3%")%numMachine %numUser %vishnu::STATUS_ACTIVE).str();
    acLogin = localAccount.getAttribut(sqlcond, "aclogin");
  }

  if(acLogin.size()==0) {
    delete account;
    delete machine;
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You do not have a local account on this machine");
  }

  delete account;
  delete machine;
  return acLogin;
}

/**
* \brief Function to check the user state (locked or not) and to
* verify the password state
*/
void
UserServer::CheckUserState(bool flagForChangePwd) {
  if (isAttributOk("status", 1)) {
    if (!flagForChangePwd) {
      if (!isAttributOk("passwordstate", 1)) {
        throw UMSVishnuException (ERRCODE_TEMPORARY_PASSWORD);
      }
    }
  } else {
    UMSVishnuException e (ERRCODE_USER_LOCKED);
    throw e;
  }
}

/**
* \brief Function to get a certain user account property
* \param machineId The machine identifier of machine on which the user have a account
* \param property The property name
* \return the user account login
*/
std::string
UserServer::getUserAccountProperty(const std::string& machineId, const std::string& property) {

  init();

  std::string userId = getData().getUserId();
  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();
  account->setMachineId(machineId);
  account->setUserId(userId);
  LocalAccountServer localAccount(account, *msessionServer);

  UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
  machine->setMachineId(machineId);
  MachineServer machineServer(machine);

  //To get the database number id of the machine
  std::string numMachine = machineServer.getAttribut("where machineid='"+localAccount.getData()->getMachineId()+"'");
  //To get the database number id of the user
  std::string numUser = getAttribut("where userid='"+localAccount.getData()->getUserId()+"'");

  std::string value;
  if ((numMachine.size() > 0) && (numUser.size() > 0)) {
    value = localAccount.getAttribut("where machine_nummachineid="+numMachine+" and users_numuserid="+numUser, property);
  }

  if(value.size()==0) {
    delete account;
    delete machine;
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You do not have a local account on this machine");
  }

  delete account;
  delete machine;
  return value;
}
