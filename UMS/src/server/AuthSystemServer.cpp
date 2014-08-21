/**
* \file AuthSystemServer.cpp
* \brief This file implements the Class which manipulates VISHNU authsystem data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "AuthSystemServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include <boost/format.hpp>

/**
* \brief Constructor
* \param authsystem The AuthSystem data structure
*/
AuthSystemServer::AuthSystemServer(UMS_Data::AuthSystem*& authsystem):
  mauthsystem(authsystem) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \param authsystem The AuthSystem data structure
* \param session The object which encapsulates session data
*/
AuthSystemServer::AuthSystemServer(UMS_Data::AuthSystem*& authsystem, SessionServer& session):
  mauthsystem(authsystem), msessionServer(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU authsystem
* \return raises an exception on error
*/
int
AuthSystemServer::add(void) {

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  checkValues();

  if (! getNumAuthSystem(mauthsystem->getAuthSystemId()).empty()) {
    throw UMSVishnuException (ERRCODE_AUTH_SYSTEM_ALREADY_EXIST);
  }

  dbSave();
  return 0;
}

/**
* \brief Function to update a VISHNU authsystem
* \return raises an exception on error
*/
int
AuthSystemServer::update() {

  std::string query = "";

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }

  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  if (! exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_SYSTEM);
  }

  if (! mauthsystem->getName().empty()) {
    query.append( (boost::format("UPDATE authsystem"
                                 " SET name='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mdatabase->escapeData(mauthsystem->getName())
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                   ).str()
                 );
  }

  if (! mauthsystem->getURI().empty()) {
    query.append( (boost::format("UPDATE authsystem"
                                 " SET uri='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mdatabase->escapeData(mauthsystem->getURI())
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                   ).str()
                  );
  }

  if (! mauthsystem->getAuthLogin().empty()) {
    query.append((boost::format("UPDATE authsystem"
                                 " SET authlogin='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mdatabase->escapeData(mauthsystem->getAuthLogin())
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                   ).str()
                 );
  }

  if (! mauthsystem->getAuthPassword().empty()) {
    query.append( (boost::format("UPDATE authsystem"
                                 " SET authpassword='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mdatabase->escapeData(mauthsystem->getAuthPassword())
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                   ).str()
                 );
  }

  if (mauthsystem->getUserPasswordEncryption() != vishnu::STATUS_UNDEFINED) {
    query.append( (boost::format("UPDATE authsystem"
                                 " SET userpwdencryption='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mauthsystem->getUserPasswordEncryption()
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())).str()
                 );
  }

  if (mauthsystem->getType() != vishnu::STATUS_UNDEFINED) {

    query.append( (boost::format("UPDATE authsystem"
                                 " SET userpwdencryption='%1%'"
                                 " WHERE numauthsystemid='%2%';")
                   % mauthsystem->getType()
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId())).str()
                 );
  }

  if (! mauthsystem->getLdapBase().empty()) {

    checkLdapBase();
    int type = vishnu::convertToInt( getEntryAttribute(mauthsystem->getAuthSystemId(), "authtype") );
    if (type != LDAPTYPE) {
      throw UMSVishnuException (ERRCODE_INVALID_PARAM, "The LDAP base option is incompatible with the authentication system type");
    }

    query.append(boost::str(
                   boost::format("UPDATE ldapauthsystem"
                                 " SET ldapbase='%1%'"
                                 " WHERE authsystem_numauthsystemid IN ("
                                 "      SELECT numauthsystemid "
                                 "        FROM authsystem "
                                 "        WHERE numauthsystemid='%2%'")
                   % mdatabase->escapeData(mauthsystem->getLdapBase())
                   % mdatabase->escapeData(mauthsystem->getAuthSystemId()))
                 );

  }


  switch (mauthsystem->getStatus()) {
    case vishnu::STATUS_LOCKED:
    case vishnu::STATUS_ACTIVE:
      query.append(boost::str(
                     boost::format("UPDATE authsystem"
                                   " SET status=%1%"
                                   " WHERE numauthsystemid='%2%';")
                     % mauthsystem->getStatus()
                     % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                     )
                   );
      break;
    default:
      break;
  }

  if (! query.empty()) {
    mdatabase->process(query);
  }

  return 0;
}

/**
* \brief Function to delete a VISHNU authsystem
* \return raises an exception on error
*/
int
AuthSystemServer::deleteAuthSystem()
{

  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  if (! userServer.exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
  }
  if (! userServer.isAdmin()) {
    throw UMSVishnuException (ERRCODE_NO_ADMIN);
  }

  //if the user-authentication system exists
  if (! exist()) {
    throw UMSVishnuException (ERRCODE_UNKNOWN_AUTH_SYSTEM);
  }

  std::string query = boost::str(
                        boost::format("UPDATE authsystem"
                                      " SET status=%1%"
                                      " WHERE numauthsystemid='%2%'"
                                      )
                        % vishnu::STATUS_DELETED
                        % mdatabase->escapeData(mauthsystem->getAuthSystemId()));

  mdatabase->process(query);

  query = boost::str(
            boost::format("UPDATE authaccount, authsystem "
                          " SET authaccount.status='%1%' "
                          " WHERE authsystem.numauthsystemid=authaccount.authsystem_numauthsystemid "
                          "  AND authsystem.numauthsystemid='%2%';")
            % vishnu::STATUS_DELETED
            % mdatabase->escapeData(mauthsystem->getAuthSystemId()));

  mdatabase->process(query);

  return 0;
}
/**
* \brief Destructor
*/
AuthSystemServer::~AuthSystemServer() {
  //delete mauthsystem;
}
/**
* \brief Function to get authsystem information
* \return  The AuthSystem data structure
*/
UMS_Data::AuthSystem *
AuthSystemServer::getData() {
  return mauthsystem;
}
/**
* \brief Get the value of a given attribute of a given NON-DELETED entry
* \param authSystemID The identifier of the auth system
* \param attr The attribute name
* \return the value of the attribut or empty string if no results
*/
std::string
AuthSystemServer::getEntryAttribute(const std::string& authSystemID, const std::string& attr) {

  std::string query = boost::str(boost::format("SELECT %1%"
                                               " FROM authsystem"
                                               " WHERE numauthsystemid='%2%'"
                                               "  AND status != %3%;")
                                 % mdatabase->escapeData(attr)
                                 % mdatabase->escapeData(authSystemID)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}


/**
* \brief Get the database ID of an auth system entry from its auth system identifier
* \param authId The auth system identifier
* \return the value as string or empty if not found
*/
std::string
AuthSystemServer::getNumAuthSystem(const std::string& authId) {

  std::string query = boost::str(boost::format("SELECT numauthsystemid"
                                               " FROM authsystem"
                                               " WHERE numauthsystemid='%1%'"
                                               "  AND status != %2%;")
                                 % mdatabase->escapeData(authId)
                                 % vishnu::STATUS_DELETED);

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query));

  return result->getFirstElement();
}

/**
* \brief Function to check the user-authentication system on database
* \return true if the authsystem exists else false
*/
bool
AuthSystemServer::exist() {
  std::string sqlcond = (boost::format("WHERE numauthsystemid = '%1%'"
                                       " AND status != %2%")
                         % mdatabase->escapeData(mauthsystem->getAuthSystemId())
                         % vishnu::STATUS_DELETED).str();
  return (!getEntryAttribute(sqlcond, "numauthsystemid").empty());
}


int
AuthSystemServer::checkValues() {

  if (mauthsystem->getType() != LDAPTYPE){
    throw UMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM_TYPE, "Invalid type");
  }

  if (mauthsystem->getType()==LDAPTYPE && (std::string(mauthsystem->getLdapBase()).size()==0)){
    throw UMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM_TYPE, "Missing ldap base");
  }
  if (mauthsystem->getType()==LDAPTYPE){
    checkLdapBase();
  }

  if (mauthsystem->getUserPasswordEncryption() != SSHA_METHOD ){
    throw UMSVishnuException(ERRCODE_UNKNOWN_ENCRYPTION_METHOD, "Invalid encryption method");
  }
  return 0;
}

/**
* \brief Function to check the $USERNAME string on the ldap base
*\return raises an exception on error
*/
int
AuthSystemServer::checkLdapBase() {
  //If the ldap base does not contain the variable $USERNAME
  if (mauthsystem->getLdapBase().find("$USERNAME") == std::string::npos) {
    throw UserException(ERRCODE_INVALID_PARAM, "No $USERNAME string found in the ldapBase");
  }
  return 0;
}


/**
 * @brief Insert the current encapsulatedd object info into database
 * @return
 */
void
AuthSystemServer::dbSave(void)
{

  mauthsystem->setStatus(vishnu::STATUS_ACTIVE);
  std::string query = boost::str(
                        boost::format("INSERT INTO authsystem"
                                      "(name,uri,authlogin,authpassword,userpwdencryption,authtype,status)"
                                      "VALUES('%1%','%2%','%3%','%4%','%5%','%6%','%7%');")
                        % mdatabase->escapeData(mauthsystem->getName())
                        % mdatabase->escapeData(mauthsystem->getURI())
                        % mdatabase->escapeData(mauthsystem->getAuthLogin())
                        % mdatabase->escapeData(mauthsystem->getAuthPassword())
                        % mauthsystem->getUserPasswordEncryption()
                        % mauthsystem->getType()
                        % mauthsystem->getStatus()
                        );

  std::pair<int, uint64_t>
      result = mdatabase->process(query);

  mauthsystem->setAuthSystemId( vishnu::convertToString(result.second) );


  if (mauthsystem->getType() == LDAPTYPE ) { // LDAP
    query = boost::str(
              boost::format("INSERT INTO ldapauthsystem (authsystem_numauthsystemid, ldapbase)"
                            " VALUES (%1%, '%2%');")
              % result.second
              % mdatabase->escapeData(mauthsystem->getLdapBase()));
    mdatabase->process(query);
  }
}
