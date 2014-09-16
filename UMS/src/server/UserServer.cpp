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
  mdatabase = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
 * \brief Constructor
 * \param user The user data structure
 */
UserServer::UserServer(const UMS_Data::User& user):muser(user) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  msessionServer = NULL;
}

/**
 * \brief Constructor
 * \param sessionServer The object to manipulate session
 */
UserServer::UserServer(SessionServer sessionServer): msessionServer(&sessionServer) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
 * \brief Function to add a new VISHNU user
 * \param user The user data structure
 * \param sendmailScriptPath The path to the script for sending emails
 * \return raises an exception on error
 */
int
UserServer::add(UMS_Data::User*& user, std::string sendmailScriptPath) {

  if (! exist()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_USER);
  }

  if (! isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  if (! getNumUserFromId(user->getUserId()).empty()) {
    throw UMSVishnuException (ERRCODE_USERID_EXISTING, user->getUserId());
  }

  //Generation of password
  std::string pwd = generatePassword(user->getLastname(), user->getFirstname());
  user->setPassword(pwd.substr(0, PASSWORD_MAX_SIZE));
  user->setStatus(vishnu::STATUS_ACTIVE);

  dbSave(*user);

  //Send email
  std::string emailBody = getMailContent(*user, true);
  sendMailToUser(*user, emailBody, "Vishnu message: user created", sendmailScriptPath);

  return 0;
}//END: add(UMS_Data::User*& user)

/**
 * \brief Function to update user information
 * \param user The user data structure
 * \return raises an exception on error
 */
int
UserServer::update(UMS_Data::User *user) {
  std::string query = "UPDATE users SET ";
  std::string comma="";
  bool updateDataProvided = false;
  bool closeAccount = false;

  if (! exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (! isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  //if a new fisrtname has been defined
  if (! user->getFirstname().empty()) {
    query.append(" firstname='"+mdatabase->escapeData(user->getFirstname())+"'");
    comma=",";
    updateDataProvided = true;
  }

  //if a new lastname has been defined
  if (!user->getLastname().empty()) {
    query.append(comma + " lastname='"+mdatabase->escapeData(user->getLastname())+"'");
    comma=",";
    updateDataProvided = true;
  }

  //if a new email has been defined
  if (!user->getEmail().empty()) {
    query.append(comma+" email='"+mdatabase->escapeData(user->getEmail())+"'");
    comma=",";
    updateDataProvided = true;
  }

  //If a new status has been defined
  if (user->getStatus() != vishnu::STATUS_UNDEFINED) {
    //if the user will be locked
    if (user->getStatus() == vishnu::STATUS_LOCKED) {
      query.append(comma + " status="+convertToString(user->getStatus())+" ");
      comma=",";
      updateDataProvided = true;
      closeAccount = true;

      //if the user is not already locked
      if (convertToInt(getEntryAttribute(user->getUserId(), "status")) == vishnu::STATUS_LOCKED) {
        std::cerr << boost::format("[WARNING] User already locked %1%\n")%user->getUserId();
      }
    } else {
      query.append(comma + " status="+convertToString(user->getStatus())+" ");
      comma=",";
      updateDataProvided = true;
    }
  }
  // Check if the user privileges should be update
  if (user->getPrivilege() != vishnu::PRIVILEGE_UNDEFINED) {
    query.append(comma+" privilege="+convertToString(user->getPrivilege())+" ");
    comma=",";
    updateDataProvided = true;
  }

  // Process the query if there is changes
  if (updateDataProvided) {
    query.append( boost::str(boost::format(" WHERE userid = '%1%'"
                                           " AND status != %2%;"
                                           )
                             % mdatabase->escapeData(user->getUserId())
                             % convertToString(vishnu::STATUS_DELETED)) );

    mdatabase->process(query);

    // close the current user sessions if the user account has been locked
    if (closeAccount) {
      query = boost::str(boost::format("UPDATE vsession"
                                       " SET state='%1%'"
                                       " WHERE sessionkey IN (SELECT sessionkey"
                                       "   FROM (SELECT users_numuserid, STATE FROM vsession) AS sessioninfo, users"
                                       "   WHERE sessioninfo.users_numuserid=users.userid"
                                       "   AND users.userid='%2%' AND sessioninfo.state='%3%');")
                         % convertToString(vishnu::SESSION_CLOSED)
                         % mdatabase->escapeData(user->getUserId())
                         % convertToString(vishnu::SESSION_ACTIVE));
      mdatabase->process(query);
    }
  }

  return 0;
}

/**
 * \brief Function to delete VISHNU user
 * \param user The user data structure
 * \return raises an exception on error
 */
int
UserServer::deleteUser(UMS_Data::User user) {
  user.setStatus(vishnu::STATUS_DELETED);
  int ret = update(&user);
  if (ret == 0) {
    std::string query = boost::str(boost::format( "UPDATE account, users"
                                                  " SET account.status='%1%'"
                                                  " WHERE users.numuserid=account.users_numuserid "
                                                  "  AND users.userid='%2%';")
                                   % vishnu::STATUS_DELETED
                                   % mdatabase->escapeData(user.getUserId()));
    return mdatabase->process(query).first;
  }
  return ret;
}//END: deleteUser(UMS_Data::User user)

/**
 * \brief Function to change VISHNU user password
 * \param newPassword The new password of the user
 * \return raises an exception on error
 */
int
UserServer::changePassword(const std::string& newPassword) {

  if (! isAuthenticate(true)) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  checkUserId(muser.getUserId());

  std::string query = boost::str(boost::format("UPDATE users SET pwd='%1%'"
                                               " WHERE userid='%2%' AND pwd='%3%' AND status != %4%;"
                                               "UPDATE users SET passwordstate=%5% "
                                               " WHERE userid='%2%' AND pwd='%1%' AND status != %4%;"
                                               )
                                 % mdatabase->escapeData( vishnu::cryptPassword(muser.getUserId(), newPassword) )
                                 % mdatabase->escapeData(muser.getUserId())
                                 % mdatabase->escapeData(muser.getPassword())
                                 % vishnu::STATUS_DELETED
                                 % vishnu::STATUS_ACTIVE);

  mdatabase->process(query);
  muser.setPassword(newPassword);

  return 0;
}

/**
 * \brief Function to change VISHNU user password
 * \param user The user data structure
 * \param sendmailScriptPath The path to the script for sending emails
 * \return raises an exception on error
 */
int
UserServer::resetPassword(UMS_Data::User& user, const std::string& sendmailScriptPath) {


  if (! exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (! isAdmin()) {
    throw UMSVishnuException(ERRCODE_NO_ADMIN);
  }

  //if the user whose password will be reset exists
  checkUserId (user.getUserId());

  //generation of a new password
  std::string pwd = generatePassword(user.getUserId(), user.getUserId());
  user.setPassword(pwd.substr(0, PASSWORD_MAX_SIZE));

  //to get the password encryptes
  std::string passwordCrypted = vishnu::cryptPassword(user.getUserId(), user.getPassword());

  //The sql code to reset the password
  std::string sqlResetPwdQuery = boost::str(boost::format("UPDATE users "
                                                          " SET pwd='%1%'"
                                                          "   WHERE userid='%2%'"
                                                          "   AND status!=%3%;"
                                                          "UPDATE users "
                                                          "  SET passwordstate=%5%"
                                                          "   WHERE userid='%2%'"
                                                          "     AND pwd='%4%'"
                                                          "     AND status!=%3%;"
                                                          )
                                            % mdatabase->escapeData(passwordCrypted)
                                            % mdatabase->escapeData(user.getUserId())
                                            % vishnu::STATUS_DELETED
                                            % mdatabase->escapeData(passwordCrypted)
                                            % vishnu::STATUS_LOCKED);

  mdatabase->process(sqlResetPwdQuery);

  user.setEmail( getEntryAttribute(user.getUserId(), "email") );

  //Send email
  sendMailToUser(user,
                 getMailContent(user, false),
                 "SysFeraDS - password reset",
                 sendmailScriptPath);

  return 0;
}

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

  //if userId and password have not been defined
  if (! muser.getUserId().empty() || ! muser.getPassword().empty()) {
    throw UMSVishnuException(ERRCODE_INVDATA, "Username or password is empty");
  }

  muser.setUserId(getAttributeFromSession("userid", msessionServer->getData().getSessionKey()));

  if (muser.getUserId().empty()) { // user not found
    throw UMSVishnuException(ERRCODE_SESSIONKEY_NOT_FOUND);
  }

  // check session state
  std::string sessionState = msessionServer->getAttributFromSessionKey(msessionServer->getData().getSessionKey(), "state");
  if (convertToInt(sessionState) != vishnu::STATUS_ACTIVE) {
    throw UMSVishnuException(ERRCODE_SESSIONKEY_EXPIRED);
  }

  muser.setPassword( getEntryAttribute(muser.getUserId(), "pwd") );
}
/**
 * \brief Function to check user on database
 * \param flagForChangePwd A flag to check the password state
 * \return true if the password state has not to be checked else false
 */
bool
UserServer::exist(bool flagForChangePwd) {

  if (! getNumUserFromLoginInfo(muser.getUserId(), muser.getPassword()).empty()) {
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
  return ! getNumUserFromLoginInfo(muser.getUserId(), muser.getPassword()).empty()
      && getEntryAttribute(muser.getUserId(), "privilege") == vishnu::convertToString(vishnu::PRIVILEGE_ADMIN);
}

/**
 * \brief Function to get user information from the database vishnu
 * \param userid The user id
 * \param attribute the attribute to get
 * \return the value of the attribut or empty string if no results
 */
std::string
UserServer::getEntryAttribute(const std::string& userid, const std::string& attribute)
{
  std::string query = boost::str(boost::format("SELECT %1% FROM users "
                                               " WHERE userid='%2%' AND status != %3%")
                                 % mdatabase->escapeData(attribute)
                                 % mdatabase->escapeData(userid)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * \brief Check if a couple userid/password is valid
 * \param userid The user id
 * \param password the user password to get
 * \return Return the num user if found. Empty otherwise
 */
std::string
UserServer::getNumUserFromLoginInfo(const std::string& userid, const std::string& password)
{
  std::string query = boost::str(boost::format("SELECT numuserid FROM users "
                                               " WHERE userid='%1%' AND pwd = '%2%' AND status != %3%")
                                 % mdatabase->escapeData(userid)
                                 % mdatabase->escapeData(password)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}



/**
 * @brief Get the DB user identifier
 * @param userid The user id
 * @return
 */
std::string
UserServer::getNumUserFromId(const std::string& userid)
{
  std::string query = boost::str(boost::format("SELECT numuserid FROM users "
                                               " WHERE userid='%1%' AND status != %2%")
                                 % mdatabase->escapeData(userid)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * \brief Function to get a user attribute from a session key
 * \param attr The attribute name
 * \param sessionKey The session key
 * \return the value of the attribut or empty string if no results
 */
std::string
UserServer::getAttributeFromSession(const std::string& attr, const std::string& sessionKey)
{
  std::string query = boost::str(boost::format("SELECT %1%"
                                               " FROM users, vsession"
                                               "  WHERE vsession.sessionkey='%2%'"
                                               "   AND users.numuserid=vsession.users_numuserid")
                                 % attr
                                 % mdatabase->escapeData(sessionKey));

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * \brief Function to check a userId
 * \param userId The userId to check
 * \return throw exception if user not found
 */
void
UserServer::checkUserId(const std::string& userId) {
  if (getEntryAttribute(userId, "userid").empty())
    throw UMSVishnuException(ERRCODE_UNKNOWN_USERID, userId);
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
                           const std::string& content,
                           const std::string& subject,
                           const std::string& sendmailScriptPath) {

  std::vector<std::string> tokens;
  std::ostringstream command;
  pid_t pid;

  std::string address = user.getEmail();
  //If the address is empty
  if (address.empty()) {
    throw UMSVishnuException(ERRCODE_INVALID_MAIL_ADRESS, "Empty email address");
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

  UMS_Data::LocalAccount_ptr account = new UMS_Data::LocalAccount();

  account->setMachineId(machineId);
  account->setUserId(getData().getUserId());
  LocalAccountServer localAccount(account, *msessionServer);

  std::string acLogin;
  if (! machineId.empty() && ! account->getUserId().empty()) {
    acLogin = localAccount.getAcLogin(machineId, account->getUserId());
  }

  if (acLogin.empty()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_LOCAL_ACCOUNT, "You do not have a local account on this machine");
  }

  delete account;
  return acLogin;
}

/**
* \brief Function to check the user state (locked or not) and to
* verify the password state
*/
void
UserServer::CheckUserState(bool flagForChangePwd) {

  int status = vishnu::convertToInt(getEntryAttribute(muser.getUserId(), "status"));
  if (status != vishnu::STATUS_ACTIVE) {
    throw UMSVishnuException(ERRCODE_USER_LOCKED);
  }

  if (! flagForChangePwd) {
    int passwordState = vishnu::convertToInt( getEntryAttribute(muser.getUserId(), "passwordstate") );
    if (passwordState != vishnu::STATUS_ACTIVE) {
      throw UMSVishnuException (ERRCODE_TEMPORARY_PASSWORD);
    }
  }
}


/**
 * @brief Save the given user object into database
 * @param user The user object
 */
void
UserServer::dbSave(const UMS_Data::User& user)
{
  std::string query = boost::str(boost::format("INSERT INTO users (userid,firstname,lastname,email, privilege,pwd,status,passwordstate) "
                                               "VALUES ('%1%','%2%','%3%','%4%','%5%','%6%','%7%', %8%)")
                                 % mdatabase->escapeData(user.getUserId())
                                 % mdatabase->escapeData(user.getFirstname())
                                 % mdatabase->escapeData(user.getLastname())
                                 % mdatabase->escapeData(user.getEmail())
                                 % user.getPrivilege()
                                 % mdatabase->escapeData(vishnu::cryptPassword(user.getUserId(), user.getPassword()) )
                                 % mdatabase->escapeData(vishnu::convertToString(user.getStatus()))
                                 % vishnu::STATUS_LOCKED);

  mdatabase->process(query);
}
