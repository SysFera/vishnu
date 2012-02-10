/**
* \file AuthSystemServer.cpp
* \brief This file implements the Class which manipulates VISHNU authsystem data on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "AuthSystemServer.hpp"
#include "DbFactory.hpp"

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
  std::string sqlInsert = "insert into authsystem (vishnu_vishnuid, "
  "authsystemid, name, uri, authlogin, authpassword, userpwdencryption, authtype, status) values ";

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();

  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {

      mauthsystem->setAuthSystemId(vishnu::getObjectId(vishnuId, "formatidauth", AUTH, ""));
      //To check if the authenid generated does no exists
      if (getAttribut("where authsystemid='"+mauthsystem->getAuthSystemId()+"'").size() == 0) {
        mdatabaseVishnu->process( sqlInsert + "(" + convertToString(vishnuId)+", "
                                  "'"+mauthsystem->getAuthSystemId()+"','"+mauthsystem->getName()+"','"
                                  + mauthsystem->getURI()+"','"+mauthsystem->getAuthLogin()+"', '"+
                                  mauthsystem->getAuthPassword() + "',"
                                  +convertToString(mauthsystem->getUserPasswordEncryption())+ ","
                                  +convertToString(mauthsystem->getType()) +", 1)"
                                );


          //If the Ldap base is defined and the type is ldap
          if ((mauthsystem->getLdapBase().size() != 0)
            && (mauthsystem->getType() == 1) ) { // 1 for LDAP

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
        if (mauthsystem->getUserPasswordEncryption() != 0) {
        sqlCommand.append("UPDATE authsystem SET userpwdencryption='"+convertToString(mauthsystem->getUserPasswordEncryption())+"'"
        " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if a type has been defined
        if (mauthsystem->getType() != 0) {
        sqlCommand.append("UPDATE authsystem SET authtype='"+convertToString(mauthsystem->getType())+"'"
        " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }

        //if a new status has been defined
        //TODO : Ajouter UNDEFINED dans statuType et dans user,machine, authenSystem
        //mettre status 3
        //Coté api quand status et à 3 dans ADD le mettre à un et dans update faire
        //un test getStatus != 3 pour machine / user/ authSystem
        //Poster bug et dans list user prendre en compte status 3 à l'affichage
         /*if (mauthsystem->getStatus().size() != 0) {
        sqlCommand.append("UPDATE authsystem SET status='"+convertToString(mauthsystem->getStatus())+"'"
        " where authsystemid='"+mauthsystem->getAuthSystemId()+"';");
        }*/

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

        mdatabaseVishnu->process(sqlCommand.c_str());
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

  std::string sqlCommand("SELECT "+attrname+" FROM authsystem "+condition);
  boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
  return result->getFirstElement();
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
