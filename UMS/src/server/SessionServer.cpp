/**
* \file SessionServer.cpp
* \brief This file implements the Class which manipulates VISHNU sessions on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "SessionServer.hpp"
#include "CommandServer.hpp"
#include "DbFactory.hpp"
#include "boost/format.hpp"


using namespace boost::posix_time;

/**
 * \brief Constructor
 */
SessionServer::SessionServer()
  : mtimeout(DEFAULT_CONNECTION_TIMEOUT)
{
  DbFactory factory;
  msession.setSessionKey("");
  mdatabase = factory.getDatabaseInstance();
}

/**
 * \brief Constructor
 * \param sessionKey The session key of the session
 * \param timeout Lenght of the connection before timeout
 */
SessionServer::SessionServer(std::string sessionKey, int timeout)
  : mtimeout(timeout)
{
  DbFactory factory;
  msession.setSessionKey(sessionKey);
  mdatabase = factory.getDatabaseInstance();
}
/**
 * \brief Constructor
 * \param session The session data structure
 * \param timeout Lenght of the connection before timeout
 */
SessionServer::SessionServer(const UMS_Data::Session& session, int timeout)
  : msession(session),
    mtimeout(timeout)
{
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}
/**
 * \brief Function to connect the session
 * \param user The object which manipulates user information
 * \param host The object which manipulates client machine information
 * \param connectOpt The options data structure for connection
 * \return raises an exception on error
 */
int
SessionServer::connectSession(UserServer user, MachineClientServer host, UMS_Data::ConnectOptions* connectOpt) {

  std::string numSubstituteUserId;
  std::string userIdToconnect;

  msession.setAuthenId( user.getData().getUserId() );

  if (! user.isAuthenticate()) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_USER);
  }

  if (! connectOpt->getSubstituteUserId().empty()) {
    if (! user.isAdmin()) {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }

    numSubstituteUserId = user.getNumUserFromId( connectOpt->getSubstituteUserId() );
    //If the user to substitute exist
    if (! user.checkUserId( connectOpt->getSubstituteUserId()) ) {
      userIdToconnect = numSubstituteUserId;
      msession.setUserId(connectOpt->getSubstituteUserId());
    } else {
      throw UMSVishnuException (ERRCODE_UNKNOWN_USERID);
    }
  }

  //if there is not a numSubstituteUserId
  if (userIdToconnect.empty()) {
    userIdToconnect = user.getNumUserFromLoginInfo(user.getData().getUserId(), user.getData().getPassword());
    msession.setUserId(user.getData().getUserId());
  }

  generateSessionKey(user.getData().getUserId());
  generateSessionId(user.getData().getUserId());

  //To solve the connection mode
  solveConnectionMode(connectOpt);

  host.recordMachineClient();
  recordSessionServer(host.getId(), userIdToconnect);

  return 0;
}//END: connectSession(UserServer, MachineClientServer, ConnectOptions*)
/**
 * \brief Function to reconnect the session
 * \param user The object which manipulates user information
 * \param host The object which manipulates client machine information
 * \param sessionId The session identifier
 * \return raises an exception on error
 */
int
SessionServer::reconnect(UserServer user, MachineClientServer clientMachineServer,
                         std::string sessionId) {
  msession.setSessionId(sessionId);

  if (! user.isAuthenticate()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  int state = getState(true);
  if (state < 0) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_SESSION_ID);
  }

  if (state != vishnu::SESSION_ACTIVE) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_SESSION_ID);
  }

  int existSessionKey = getSessionkey(clientMachineServer.getId(),
                                      user.getNumUserFromLoginInfo(user.getData().getUserId(),user.getData().getPassword()),
                                      user.isAdmin());

  if (existSessionKey == -1) {
    throw UMSVishnuException (ERRCODE_SESSIONKEY_NOT_FOUND);
  }

  return 0;
}


/**
* \brief Function to close the session
* \return raises an exception on error
*/
int
SessionServer::close() {
  UserServer user = UserServer(SessionServer(msession.getSessionKey()));
  CommandServer commanderServer = CommandServer(SessionServer(msession.getSessionKey()));

  // initialize and check the user
  user.init();
  if (user.exist()) {
    int state = getState();
    if (state != vishnu::SESSION_CLOSED) {
      if (! commanderServer.isRunning()) {
        mdatabase->process(boost::str(boost::format("UPDATE vsession"
                                                    " SET state=%1%"
                                                    " WHERE sessionkey='%1%';")
                                      % vishnu::SESSION_CLOSED
                                      % mdatabase->escapeData(msession.getSessionKey())));

        mdatabase->process(boost::str(boost::format("UPDATE vsession"
                                                    " SET closure=CURRENT_TIMESTAMP"
                                                    " WHERE sessionkey='%1%';")
                                      % mdatabase->escapeData(msession.getSessionKey())));
      } else {
        int closePolicy = vishnu::convertToInt(getAttributFromSessionKey(msession.getSessionKey(), "closepolicy"));
        if (closePolicy == vishnu::CLOSE_ON_DISCONNECT) {
          disconnetToTimeout();
        } else {
          throw  UMSVishnuException (ERRCODE_COMMAND_RUNNING);
        }
      }
    } else {
      throw UMSVishnuException (ERRCODE_SESSIONKEY_EXPIRED);
    }
  }
  return 0;
}

/**
 * \brief Destructor
 */
SessionServer::~SessionServer() {
}
/**
 * \brief Function to get the session information
 * \return  The session data structure
 */
UMS_Data::Session
SessionServer::getData() const {
  return msession;
}

/**
 * \brief Get session attribute from sessionkey
 * \param sessionKey The session key
 * \param attrname the name of the attribut to get
 * \return the value of the attribut or empty string if no results
 */
std::string
SessionServer::getAttributFromSessionKey(const std::string& sessionKey, const std::string& attrname)
{
  std::string query = boost::str(boost::format("SELECT %1% FROM vsession"
                                               " WHERE sessionkey='%2%'")
                                 % mdatabase->escapeData(attrname)
                                 % mdatabase->escapeData(sessionKey));

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}


/**
 * \brief Get session attribute from sessionid
 * \param sessionId The session key
 * \param attrname the name of the attribut to get
 * \return the value of the attribut or empty string if no results
 */
std::string
SessionServer::getAttributFromSessionId(const std::string& sessionKey, const std::string& attrname)
{
  std::string query = boost::str(boost::format("SELECT %1% FROM vsession"
                                               " WHERE vsessionid='%2%'")
                                 % mdatabase->escapeData(attrname)
                                 % mdatabase->escapeData(sessionKey));

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
 * \brief Function to save the date of the last connection
 * \return raises an exception on error
 */
int
SessionServer::saveConnection() {

  std::string sqlCommand = "UPDATE vsession SET lastconnect=CURRENT_TIMESTAMP"
                           " WHERE sessionkey='"+mdatabase->escapeData(msession.getSessionKey())+"'";

  mdatabase->process(sqlCommand);
  return 0;
}

/**
 * \brief Function to get the list of sessions with close on timeout mode to close
 * \return the list of results
 */
DatabaseResult*
SessionServer::getSessionToclosebyTimeout()
{
  DatabaseResult* result;
  std::string query;

  switch(mdatabase->getDbType()) {
    case DbConfiguration::MYSQL:
      query = "SELECT sessionkey from vsession where "
              " unix_timestamp(CURRENT_TIMESTAMP) - unix_timestamp(lastconnect) > timeout and state=1 "
              " and closepolicy=1";
      break;
    case DbConfiguration::POSTGRESQL:
      query = "SELECT sessionkey from vsession where "
              " EXTRACT( epoch FROM  CURRENT_TIMESTAMP ) - EXTRACT( epoch FROM lastconnect ) > timeout and state=1 "
              " and closepolicy=1";
      break;
    case DbConfiguration::ORACLE:
      throw SystemException(ERRCODE_DBERR, "SessionServer::getSessionToclosebyTimeout: Oracle query not defined");
      break;
    default:
      break;
  }

  result = mdatabase->getResult(query);

  return result;

}

/**
 * \brief Function to make a complete checking of the session key
 * \return raises an exception on error
 */
int
SessionServer::check() {

  std::string query = boost::str(boost::format("SELECT state, status, passwordstate"
                                               " FROM users, vsession "
                                               " WHERE users.numuserid = vsession.users_numuserid"
                                               " AND vsession.sessionkey='%1%'"
                                               " AND vsession.state!=%2%"
                                               )
                                 % mdatabase->escapeData(msession.getSessionKey())
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));
  if (result->getNbTuples() == 0) {
    throw UMSVishnuException (ERRCODE_SESSIONKEY_NOT_FOUND);
  }

  std::vector<std::string> resultEntry = result->get(0);
  if (vishnu::convertToInt(resultEntry[0]) != vishnu::SESSION_ACTIVE) {
    throw  UMSVishnuException (ERRCODE_SESSIONKEY_EXPIRED);
  }

  if (vishnu::convertToInt(resultEntry[1]) != vishnu::STATUS_ACTIVE) {
    throw UMSVishnuException (ERRCODE_USER_LOCKED);
  }

  if (vishnu::convertToInt(resultEntry[2]) != vishnu::STATUS_ACTIVE) {
    throw UMSVishnuException (ERRCODE_TEMPORARY_PASSWORD);
  }

  return 0;
}

/**
* \brief Function to finalize the service
* \param cmdDescription The description of the command
* \param cmdType The type of the command (UMS, TMS, FMS)
* \param cmdStatus The status of the command
* \param newVishnuObjectID the new vishnu object
* \return raises an exception on error
*/

int
SessionServer::finish(std::string cmdDescription,
                      vishnu::CmdType cmdType,
                      vishnu::CmdStatus cmdStatus,
                      const std::string& newVishnuObjectID,
                      bool checkSession) {

  //To save the date of the last connection
  if (checkSession) {
    check();
  }
  saveConnection();
  //To save the command
  CommandServer commandServer = CommandServer(cmdDescription, *this);
  commandServer.record(cmdType, cmdStatus, newVishnuObjectID);
  return 0;
}


/**
 * \brief Function to generate the session key
 * \param salt This string is used to perturb the algorithm
 * \return an encrypted message registered on the session data structure
 */
int
SessionServer::generateSessionKey(std::string salt) {

  std::string sessionKey;
  //the current time
  ptime now = microsec_clock::local_time();

  //for the md5 encryption
  std::string tmpSalt = "$1$" + salt + "$";

  //for SHA1-512 encryption by using the userId as a salt
  std::string globalSalt = "$6$"+std::string(crypt(salt.c_str(), tmpSalt.c_str())).substr(tmpSalt.size())+"$";

  //SHA1-512 encryption of the salt encrypted using the md5 and the current time as the clef
  sessionKey = std::string(crypt(to_simple_string(now).c_str(), globalSalt.c_str()));

  //To put a randomize number at the end to complicate the string encrypted
  sessionKey.append(vishnu::convertToString(vishnu::generateNumbers()));
  msession.setSessionKey(sessionKey.substr(globalSalt.size()));

  return 0;
}
/**
 * \brief Function to generate the session identifier
 * \param userId the userId of the owner of the session
 * \return record the the session id on the session data structure
 */
int
SessionServer::generateSessionId(std::string userId) {

  std::string sessionId;

  //the current time
  ptime now = microsec_clock::local_time();

  //To get the current time as a string
  std::string nowToString = to_simple_string(now.date());
  nowToString.append("-"+to_simple_string(now.time_of_day()));

  //To construct the session key
  sessionId.append(userId+"-");
  sessionId.append(nowToString+":"+vishnu::convertToString( vishnu::generateNumbers() ));

  msession.setSessionId(sessionId);

  return 0;
}
/**
 * \brief Function to record the session on the database
 * \param idmachine the database number id of the client machine
 * \param iduser the database number id of the owner of the session
 * \return record the the session id on the session data structure
 */
int
SessionServer::recordSessionServer(std::string idmachine, std::string numuser) {

  std::string query = boost::str(
                        boost::format("INSERT INTO vsession "
                                      " (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, "
                                      "  creation, sessionKey, state, closepolicy, timeout, authid) "
                                      " VALUES ('%1%','%2%','%3%',CURRENT_TIMESTAMP,CURRENT_TIMESTAMP,'%4%','%5%','%6%','%7%','%8%')"
                                      )
                        % mdatabase->escapeData(msession.getSessionId())
                        % idmachine
                        % numuser
                        % mdatabase->escapeData(msession.getSessionKey())
                        % msession.getStatus()
                        % msession.getClosePolicy()
                        % msession.getTimeout()
                        % msession.getAuthenId());

  return mdatabase->process(query).first;
}
/**
 * \brief Function to check the session on database
 * \param flagSessionId A flag to check the session id
 * \return true if the session key (or sessionId) exists else false
 */
bool
SessionServer::exist(bool flagSessionId) {

  if (flagSessionId) {
    return (getState(true) != -1);
  }
  else {
    return (getState() != -1);
  }

}


/**
 * \brief Function to check the session on database
 * \param flagSessionId A flag to check the session identifier
 * \return the session status
 */
int
SessionServer::getState(bool flagSessionId) {

  std::string sessionState;

  if (flagSessionId) {
    sessionState = getAttributFromSessionId(msession.getSessionId(), "state");
  } else {
    sessionState = getAttributFromSessionKey(msession.getSessionKey(), "state");
  }

  return sessionState.empty()? -1 : vishnu::convertToInt(sessionState);
}
/**
 * \brief Function to check the session on database
 * \param nummachine the database number id of the client machine
 * \param numuser the database number id of the owner of the session
 * \param flagAdmin A flag for an admin to get the session key from another user
 * \return 0 on success and -1 if there is no results
 */
int
SessionServer::getSessionkey(std::string nummachine, std::string numuser, bool flagAdmin) {

  std::string key = "";
  std::string closePolicyStr = "";

  //if the user is not an admin, the machine and the userid are checked
  if (! flagAdmin) {
    std::string query = boost::str(boost::format("SELECT sessionkey FROM vsession"
                                                 " WHERE vsessionid='%1%'"
                                                 "   AND clmachine_numclmachineid='%2%'"
                                                 "   AND users_numuserid='%3%'")
                                   % mdatabase->escapeData(msession.getSessionId())
                                   % mdatabase->escapeData(nummachine)
                                   % mdatabase->escapeData(numuser));

    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));
    key = result->getFirstElement();
  } else {
    key = getAttributFromSessionId(msession.getSessionId(), "sessionkey");
  }

  if (! key.empty()) {
    msession.setSessionKey(key);
    closePolicyStr =  getAttributFromSessionId(msession.getSessionId(), "closepolicy");
    msession.setClosePolicy( vishnu::convertToInt(closePolicyStr) );
    return 0;
  }

  return -1;
}


/**
 * \brief Function to solve the session connection parameters
 )
 * \param connectOpt the connection parameters
 * \return the connection parameters are registered on the session data structure
 */
int
SessionServer::solveConnectionMode(UMS_Data::ConnectOptions* connectOpt) {
  switch (connectOpt->getClosePolicy()) {
    ////The closure mode is default
    case vishnu::DEFAULT_CONNECTION_CLOSE_POLICY :
      msession.setClosePolicy(vishnu::DEFAULT_CONNECTION_CLOSE_POLICY);
      if (msession.getClosePolicy() != vishnu::CLOSE_ON_DISCONNECT) {
        msession.setTimeout(DEFAULT_CONNECTION_TIMEOUT);
      }
      break;
    case vishnu::CLOSE_ON_TIMEOUT:
      msession.setClosePolicy(vishnu::CLOSE_ON_TIMEOUT);
      if (connectOpt->getSessionInactivityDelay() != 0) {
        msession.setTimeout(connectOpt->getSessionInactivityDelay());
      } else {
        msession.setTimeout(mtimeout);
      }
      break;
    case vishnu::CLOSE_ON_DISCONNECT:
      msession.setClosePolicy(vishnu::CLOSE_ON_DISCONNECT);
      break;

    default:
      throw SystemException(ERRCODE_SYSTEM, "Invalid close policy value in SessionServer::solveConnectionMode");
  }
  return 0;
}

/**
 * \brief Function to change the closure connection mode disconnet to timeout
 * \return the new connection parameters are registered on the session data structure
 */
int
SessionServer::disconnetToTimeout() {

  //To change the session close policy on CLOSE_ON_TIMEOUT on the database
  mdatabase->process(boost::str(boost::format("UPDATE vsession SET closepolicy=%1%"
                                              " WHERE sessionkey='%2%';")
                                % vishnu::CLOSE_ON_TIMEOUT
                                % mdatabase->escapeData(msession.getSessionKey())));

  msession.setClosePolicy(vishnu::CLOSE_ON_TIMEOUT);
  msession.setTimeout(mtimeout);
  mdatabase->process(boost::str(boost::format("UPDATE vsession SET closepolicy=%1%"
                                              " WHERE sessionkey='%2%';")
                                % msession.getTimeout()
                                % mdatabase->escapeData(msession.getSessionKey())));

  return 0;
}
