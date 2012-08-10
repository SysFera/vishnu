/**
* \file AuthSystemServer.cpp
* \brief This file implements the Class which manipulates VISHNU authsystem data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "AuthSystemServer.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"

/**
* \brief Constructor
* \fn AuthSystemServer(AuthSystem authsystem, SessionServer session)
* \param authsystem The AuthSystem data structure
*/
AuthSystemServer::AuthSystemServer(UMS_Data::AuthSystem*& authsystem):
mauthsystem(authsystem) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Constructor
* \fn AuthSystemServer(AuthSystem authsystem, SessionServer session)
* \param authsystem The AuthSystem data structure
* \param session The object which encapsulates session data
*/
AuthSystemServer::AuthSystemServer(UMS_Data::AuthSystem*& authsystem, SessionServer& session):
mauthsystem(authsystem), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
* \brief Function to add a new VISHNU authsystem
* \fn int add()
* \param vishnuId The identifier of the vishnu instance
* \return raises an exception on error
*/
int
AuthSystemServer::add(int vishnuId) {

  std::string numAuth;
  std::string sqlUpdate = "UPDATE authsystem set ";
  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      checkValues();
      mauthsystem->setAuthSystemId(vishnu::getObjectId(vishnuId, "formatidauth", AUTH, ""));
      // To check if the authentication id generated and the name to save do not exist,
      // except the authentication reserved by getObjectId
      if (getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId(), "count(*)") == "1"
    		  && getAttribut("where name='"+mauthsystem->getName()+"'").empty() ) {
        //To active the user-authentication system
        mauthsystem->setStatus(ACTIVE_STATUS);

        sqlUpdate+="name='"+mauthsystem->getName()+"',";
        sqlUpdate+="uri='"+mauthsystem->getURI()+"',";
        sqlUpdate+="authlogin='"+mauthsystem->getAuthLogin()+"',";
        sqlUpdate+="authpassword='"+mauthsystem->getAuthPassword()+"',";
        sqlUpdate+="userpwdencryption="+convertToString(mauthsystem->getUserPasswordEncryption())+",";
        sqlUpdate+="authtype="+convertToString(mauthsystem->getType())+",";
        sqlUpdate+="status="+convertToString(mauthsystem->getStatus())+" ";
        sqlUpdate+="WHERE authsystemid='"+mauthsystem->getAuthSystemId()+"';";
        mdatabaseVishnu->process(sqlUpdate);

          //If the Ldap base is defined and the type is ldap
          if (mauthsystem->getType() == LDAPTYPE ) { // LDAP

            numAuth = getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId()+"'");

            mdatabaseVishnu->process("insert into ldapauthsystem (authsystem_authsystemid, ldapbase) values "
                                    "("+numAuth+ ", '"+mauthsystem->getLdapBase()+"')");
          }

      }// End if the id generated does not exists
      else {
        UMSVishnuException e (ERRCODE_AUTH_SYSTEM_ALREADY_EXIST);
        throw e;
      }
    }//End if the user is an admin
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
* \brief Function to update a VISHNU authsystem
* \fn int update()
* \return raises an exception on error
*/
int
AuthSystemServer::update() {

  std::string sqlCommand = "";

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      //if the user-authentication system exists
      if (exist()) {
        //if a new name has been defined
        if (mauthsystem->getName().size() != 0) {
          sqlCommand.append("UPDATE authsystem SET name='"+mauthsystem->getName()+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if an URI has been defined
        if (mauthsystem->getURI().size() != 0) {
          sqlCommand.append("UPDATE authsystem SET uri='"+mauthsystem->getURI()+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if an authLogin has been defined
        if (mauthsystem->getAuthLogin().size() != 0) {
          sqlCommand.append("UPDATE authsystem SET authlogin='"+mauthsystem->getAuthLogin()+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if an authPassword has been defined
        if (mauthsystem->getAuthPassword().size() != 0) {
          sqlCommand.append("UPDATE authsystem SET authpassword='"+mauthsystem->getAuthPassword()+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if a password encryption method has been defined
        if (mauthsystem->getUserPasswordEncryption() != UNDEFINED_VALUE) {
          sqlCommand.append("UPDATE authsystem SET userpwdencryption='"+convertToString(mauthsystem->getUserPasswordEncryption())+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if a type has been defined
        if (mauthsystem->getType() != UNDEFINED_VALUE) {
          sqlCommand.append("UPDATE authsystem SET authtype='"+convertToString(mauthsystem->getType())+"'"
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //If an ldap base has been defined
        if (mauthsystem->getLdapBase().size() != 0) {

          checkLdapBase();
          std::string type = getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId()+"'", "authtype");
          //If the authentication system is not an ldap type
          if (convertToInt(type) != LDAPTYPE) {
            UMSVishnuException e (ERRCODE_INVALID_PARAM, "The ldap base option is incompatible with the user"
            " authentication system type");
            throw e;
          }

          sqlCommand.append("UPDATE ldapauthsystem SET ldapbase='"+mauthsystem->getLdapBase()+"'"
          " where authsystem_authsystemid IN (SELECT numauthsystemid from authsystem where authsystemid='"+mauthsystem->getAuthSystemId()+"');");

        }


        //if a new status has been defined
        if (mauthsystem->getStatus() != UNDEFINED_VALUE) {
          //if the authsystem will be locked
          if (mauthsystem->getStatus() == 0) {
            //if the authsystem is not already locked
            if (convertToInt(getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId()+"'", "status")) != 0) {
              sqlCommand.append("UPDATE authsystem SET status="+convertToString(mauthsystem->getStatus())+""
              " where  authsystemid='"+mauthsystem->getAuthSystemId()+"';");
            } //End if the user is not already locked
            else {
              UMSVishnuException e (ERRCODE_AUTH_SYSTEM_ALREADY_LOCKED);
              throw e;
            }
          } //End if the authsystem will be locked
          else {
            sqlCommand.append("UPDATE authsystem SET status="+convertToString(mauthsystem->getStatus())+""
            " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
          }
        }

        //If there is a change
        if (!sqlCommand.empty()) {
          mdatabaseVishnu->process(sqlCommand.c_str());
        }
      } //End if the user-authentication system exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_SYSTEM);
        throw e;
      }
    }//End if the user is admin
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
* \brief Function to delete a VISHNU authsystem
* \fn int deleteAuthSystem()
* \return raises an exception on error
*/
int
AuthSystemServer::deleteAuthSystem() {

  //Creation of the object use
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      //if the user-authentication system exists
      if (exist()) {
          //To remove authentication system from vishnu
          mdatabaseVishnu->process("DELETE FROM authsystem "
          " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
      } //End if the user-authentication system exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_AUTH_SYSTEM);
        throw e;
      }
    }//End if the user is admin
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
* \fn ~AuthSystemServer
* \brief Destructor
*/
AuthSystemServer::~AuthSystemServer() {
  //delete mauthsystem;
}
/**
* \brief Function to get authsystem information
* \fn UMS_Data::AuthSystem getData()
* \return  The AuthSystem data structure
*/
UMS_Data::AuthSystem *
AuthSystemServer::getData() {
  return mauthsystem;
}
/**
* \brief Function to get authsystem information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string
AuthSystemServer::getAttribut(std::string condition, std::string attrname) {
	std::string ret;
	std::string sqlCommand("SELECT "+attrname+" FROM authsystem "+condition);
	SOCISession session = mdatabaseVishnu->getSingleSession();
	(session<<sqlCommand).into(ret);
	mdatabaseVishnu->releaseSingleSession(session);
	return ret;
}

/**
* \brief Function to check the user-authentication system on database
* \fn bool exist()
* \return true if the authsystem exists else false
*/
bool
AuthSystemServer::exist() {
  return (getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId()+"'").size() != 0);
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
