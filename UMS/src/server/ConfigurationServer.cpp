/**
 * \file ConfigurationServer.cpp
 * \brief This file implements the Class which manipulates VISHNU configuration data on server side.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2011
 */

#include "ConfigurationServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "AuthAccountServer.hpp"
#include "AuthSystemServer.hpp"
#include "DbFactory.hpp"

/**
 * \brief Constructor
 * \fn ConfigurationServer(SessionServer session)
 * \param session The object which encapsulates session data
 */
ConfigurationServer::ConfigurationServer(SessionServer session):msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
  mconfiguration = NULL;

}
/**
 * \brief Constructor
 * \fn ConfigurationServer(Configuration*& configuration, SessionServer session);
 * \param configuration The Configuration data structure
 * \param session The object which encapsulates session data
 */
ConfigurationServer::ConfigurationServer(UMS_Data::Configuration*& configuration, SessionServer session)
:mconfiguration(configuration), msessionServer(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance();
}

/**
 * \brief Function to save a VISHNU configuration
 * \fn int save()
 * \return raises an exception on error
 */
int
ConfigurationServer::save() {

  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users "
  "where not userid='"+ROOTUSERNAME+"'";

  std::string sqlListofMachines = "SELECT machineid, name, site, status, sshpublickey, lang, description from machine, description "
  "where machine.nummachineid = description.machine_nummachineid";

  std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home "
  "from account, machine, users where account.machine_nummachineid=machine.nummachineid and "
  "account.users_numuserid=users.numuserid";

  std::string sqlListofAuthSystems = "SELECT authsystemid, name, uri, authlogin, authpassword, userpwdencryption, authtype, status, ldapbase from authsystem, ldapauthsystem "
                                     " where ldapauthsystem.authsystem_authsystemid = authsystem.numauthsystemid";

  std::string sqlListofAuthAccounts = "SELECT authsystemid, userid, aclogin "
    " from authaccount, authsystem, users where authaccount.authsystem_authsystemid=authsystem.numauthsystemid and authaccount.users_numuserid=users.numuserid";

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mconfiguration= ecoreFactory->createConfiguration();

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the user exists
  if (userServer.exist()) {
    //if the user is an admin
    if (userServer.isAdmin()) {
      //To get the list of users from the database
      boost::scoped_ptr<DatabaseResult> ListofUsers (mdatabaseVishnu->getResult(sqlListofUsers.c_str()));

      if (ListofUsers->getNbTuples() != 0){
        for (size_t i = 0; i < ListofUsers->getNbTuples(); ++i) {
          results.clear();
          results = ListofUsers->get(i);
          ii = results.begin();

          UMS_Data::User_ptr user = ecoreFactory->createUser();
          user->setUserId(*ii);
          user->setPassword(*(++ii));
          user->setFirstname(*(++ii));
          user->setLastname(*(++ii));
          user->setPrivilege(convertToInt(*(++ii)));
          user->setEmail(*(++ii));
          //Add 1 on status because of the storage of EMF litteral on file which is shifted
          user->setStatus(1+convertToInt(*(++ii)));
          mconfiguration->getListConfUsers().push_back(user);
        }
      }

      //To get the list of machines from the database
      boost::scoped_ptr<DatabaseResult> ListofMachines (mdatabaseVishnu->getResult(sqlListofMachines.c_str()));

      if (ListofMachines->getNbTuples() != 0){
        for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
          results.clear();
          results = ListofMachines->get(i);
          ii = results.begin();

          UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
          machine->setMachineId(*ii);
          machine->setName(*(++ii));
          machine->setSite(*(++ii));
          //Add 1 on status because of the storage of EMF litteral on file which is shifted
          machine->setStatus(1+convertToInt(*(++ii)));
          machine->setSshPublicKey(*(++ii));
          machine->setLanguage(*(++ii));
          machine->setMachineDescription(*(++ii));
          mconfiguration->getListConfMachines().push_back(machine);
        }
      }

      //To get the list of local accounts from the database
      boost::scoped_ptr<DatabaseResult> ListofLocalAccount (mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str()));

      if (ListofLocalAccount->getNbTuples() != 0){
        for (size_t i = 0; i < ListofLocalAccount->getNbTuples(); ++i) {
          results.clear();
          results = ListofLocalAccount->get(i);
          ii = results.begin();

          UMS_Data::LocalAccount_ptr localAccount = ecoreFactory->createLocalAccount();
          localAccount->setMachineId(*ii);
          localAccount->setUserId(*(++ii));
          localAccount->setAcLogin(*(++ii));
          localAccount->setSshKeyPath(*(++ii));
          localAccount->setHomeDirectory(*(++ii));
          mconfiguration->getListConfLocalAccounts().push_back(localAccount);
        }
      }

      //To get the list of user-authentication systems from the database
      boost::scoped_ptr<DatabaseResult> ListofAuthSystems (mdatabaseVishnu->getResult(sqlListofAuthSystems.c_str()));

      if (ListofAuthSystems->getNbTuples() != 0){
        for (size_t i = 0; i < ListofAuthSystems->getNbTuples(); ++i) {
          results.clear();
          results = ListofAuthSystems->get(i);
          ii = results.begin();

          UMS_Data::AuthSystem_ptr authSystem = ecoreFactory->createAuthSystem();
          authSystem->setAuthSystemId(*ii);
          authSystem->setName(*(++ii));
          authSystem->setURI(*(++ii));
          authSystem->setAuthLogin(*(++ii));
          authSystem->setAuthPassword(*(++ii));
          //Add 1 for all enum types because of the storage of EMF litteral on file which is shifted
          authSystem->setUserPasswordEncryption(1+convertToInt(*(++ii)));
          authSystem->setType(1+convertToInt(*(++ii)));
          authSystem->setStatus(1+convertToInt(*(++ii)));
          authSystem->setLdapBase(*(++ii));
          mconfiguration->getListConfAuthSystems().push_back(authSystem);
        }
      }

      //To get the list of user-authentication systems from the database
      boost::scoped_ptr<DatabaseResult> ListofAuthAccounts (mdatabaseVishnu->getResult(sqlListofAuthAccounts.c_str()));

      if (ListofAuthAccounts->getNbTuples() != 0){
        for (size_t i = 0; i < ListofAuthAccounts->getNbTuples(); ++i) {
          results.clear();
          results = ListofAuthAccounts->get(i);
          ii = results.begin();

          UMS_Data::AuthAccount_ptr authAccount = ecoreFactory->createAuthAccount();
          authAccount->setAuthSystemId(*ii);
          authAccount->setUserId(*(++ii));
          authAccount->setAcLogin(*(++ii));
          mconfiguration->getListConfAuthAccounts().push_back(authAccount);
        }
      }
    } //End if the user is an admin
    else {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }
  }//End //if the user exists
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return 0;
}

/**
 * \brief Function to restore a VISHNU configuration
 * \fn int restore(int vishnuId)
 * \param vishnuId The identifier of the vishnu instance
 * \return raises an exception on error
 */
int ConfigurationServer::restore(int vishnuId) {
  std::string sqlcode = "";
  std::string sqlCodeDescMachine = "";
  std::string sqlcodeAuthSystem = "";
  std::string sqlcodeLdapAuthSystem = "";
  std::string cleanAllModelInfo  = "delete from parameter_value; delete from work; delete from project_machine;  delete from user_role; delete from project_role_permissions;"
  "delete from project_member_project_role; delete from project_member; delete from project_role; delete from project_application; delete from notification;"
  "delete from application_parameter; delete from application_version; delete from application; delete from project; delete from acl_entry;"
  "delete from acl_object_identity; delete from acl_class; delete from acl_sid; delete from role; delete from global_project_role_permissions;"
  "delete from global_project_role; delete from permission; delete from test_report; delete from permission_module; ";

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the userid is the super vishnu admin userid
  if (userServer.getData().getUserId().compare(ROOTUSERNAME) == 0) {
    //if the user exists
    if (userServer.exist()) {

      mdatabaseVishnu->process(cleanAllModelInfo+" DELETE FROM users where not userid='"+ROOTUSERNAME+"';"
      "DELETE FROM machine; DELETE FROM account; DELETE FROM authsystem; delete FROM authaccount;");

      //To get all users
      for(unsigned int i = 0; i < mconfiguration->getListConfUsers().size(); i++) {
        UMS_Data::User_ptr user = mconfiguration->getListConfUsers().get(i);
        //userServer.add(user);
        sqlcode.append(userToSql(user, vishnuId));
      }

      //To get all machines
      for(unsigned int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
        sqlcode.append(machineToSql(machine, vishnuId));
      }

      //To insert machines and users
      mdatabaseVishnu->process(sqlcode.c_str());

      //To get machines description
      for(unsigned int i = 0; i < mconfiguration->getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = mconfiguration->getListConfMachines().get(i);
        sqlCodeDescMachine.append(machineDescToSql(machine));
      }

      //To insert machines description
      mdatabaseVishnu->process(sqlCodeDescMachine.c_str());

      //To insert localAccount
      for(unsigned int i = 0; i < mconfiguration->getListConfLocalAccounts().size(); i++) {
        UMS_Data::LocalAccount_ptr localAccount = mconfiguration->getListConfLocalAccounts().get(i);
        LocalAccountServer localAccountServer = LocalAccountServer (localAccount, msessionServer);
        localAccountServer.add();
      }

       //To get all user-authentication systems
      for(unsigned int i = 0; i < mconfiguration->getListConfAuthSystems().size(); i++) {
        UMS_Data::AuthSystem_ptr authsystem = mconfiguration->getListConfAuthSystems().get(i);
        sqlcodeAuthSystem.append(authSystemToSql(authsystem, vishnuId));
      }

      //If authSystem has been defined
      if (!sqlcodeAuthSystem.empty()) {
        //To insert user-authentication systems
        mdatabaseVishnu->process(sqlcodeAuthSystem.c_str());

        //To get all ldap systems in a second time in order to be
        //sure that recording has been done before
        for(unsigned int i = 0; i < mconfiguration->getListConfAuthSystems().size(); i++) {
          UMS_Data::AuthSystem_ptr authsystem = mconfiguration->getListConfAuthSystems().get(i);
          sqlcodeLdapAuthSystem.append(ldapAuthSystemToSql(authsystem));
        }
        //If ldapsystem has been defined
        if (!sqlcodeLdapAuthSystem.empty()) {
          mdatabaseVishnu->process(sqlcodeLdapAuthSystem.c_str());
        }

        //To insert authAccount
        for(unsigned int i = 0; i < mconfiguration->getListConfAuthAccounts().size(); i++) {
          UMS_Data::AuthAccount_ptr authAccount = mconfiguration->getListConfAuthAccounts().get(i);
          AuthAccountServer authAccountServer = AuthAccountServer (authAccount, msessionServer);
          authAccountServer.add();
        }
      }
    } //End if the user exists
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
  }//End if the userid is the super vishnu admin userid
  else {
    UMSVishnuException e (ERRCODE_ROOT_USER_ONLY);
    throw e;
  }
  return 0;
}

/**
 * \fn ~ConfigurationServer()
 * \brief Destructor
*/
ConfigurationServer::~ConfigurationServer() {
  delete mconfiguration;
}
/**
 * \brief Function to get Configuration data structure
 * \fn UMS_Data::Configuration*  getData()
 * \return  The LocalAccount data structure
 */
UMS_Data::Configuration*
ConfigurationServer::getData() {
  return mconfiguration;
}

/**
 * \brief Function to get the sql code of users from a VISHNU configuration
 * \fn std::string userToSql(UMS_Data::User_ptr user, int vishnuId)
 * \param vishnuId The identifier of the vishnu instance
 * \return the sql code containing the sql code of users
 */
std::string
ConfigurationServer::userToSql(UMS_Data::User_ptr user, int vishnuId) {

  std::string sqlInsert = "insert into users (vishnu_vishnuid, userid, pwd, firstname, lastname,"
  " privilege, email, passwordstate, status) values ";

  //Remove 1 on status because of the storage of EMF litteral on file which is shifted of 1
  return (sqlInsert + "(" + convertToString(vishnuId) +", "
  "'"+user->getUserId()+"','"+user->getPassword()+"','"
  + user->getFirstname()+"','"+user->getLastname()+"',"+
  convertToString(user->getPrivilege()) +",'"+user->getEmail() +"', "
  "1, "+convertToString(user->getStatus()-1)+");");
}

/**
 * \brief Function to get the sql code of machines from a VISHNU configuration
 * \fn std::string machineToSql(UMS_Data::Machine_ptr machine, int vishnuId)
 * \param machine The machine object
 * \param vishnuId The identifier of the vishnu instance
 * \return the sql code containing the sql code of machines
 */
std::string
ConfigurationServer::machineToSql(UMS_Data::Machine_ptr machine, int vishnuId) {
  std::string sqlInsert = "insert into machine (vishnu_vishnuid, name, site, machineid, status, sshpublickey) values ";

  //Remove 1 on status because of the storage of EMF litteral on file which is shifted of 1
  sqlInsert.append("("+convertToString(vishnuId)+",'"+machine->getName()+"'"
  ",'"+ machine->getSite()+"','"+machine->getMachineId()+"',"
  +convertToString(machine->getStatus()-1)+",'"+machine->getSshPublicKey() +"');");

  return sqlInsert;
}

/**
 * \brief Function to get the sql code of machines description from a VISHNU configuration
 * \fn std::string machineDescToSql(UMS_Data::Machine_ptr machine)
 * \return the sql code containing the sql code of machines description
 */
std::string
ConfigurationServer::machineDescToSql(UMS_Data::Machine_ptr machine) {

  UMS_Data::Machine* machinetmp = new UMS_Data::Machine();
  MachineServer machineServer = MachineServer(machinetmp);
  std::string res;
  res = "insert into description (machine_nummachineid, lang, "
  "description) values "
  "("+machineServer.getAttribut("where machineid='"+machine->getMachineId()+"';")+","
  "'"+ machine->getLanguage()+"','"+machine->getMachineDescription()+"');";

  delete machinetmp;
  return res;
}

/**
* \brief Function to get the sql code of user-authentication system from a VISHNU configuration
* \fn authSystemToSql(UMS_Data::AuthSystem_ptr authSystem, int vishnuId)
* \param authSystem The authSystem object
* \param vishnuId The identifier of the vishnu instance
* \return the sql code containing the sql code of authentication systems
*/
std::string
ConfigurationServer::authSystemToSql(UMS_Data::AuthSystem_ptr authsystem, int vishnuId) {
  std::string sqlInsert= "insert into authsystem (vishnu_vishnuid, "
  "authsystemid, name, uri, authlogin, authpassword, userpwdencryption, authtype, status) values ";

  //Remove 1 on enum types because of the storage of EMF litteral on file which is shifted of 1
  return (sqlInsert + "(" + convertToString(vishnuId)+", "
                                  "'"+authsystem->getAuthSystemId()+"','"+authsystem->getName()+"','"
                                  + authsystem->getURI()+"','"+authsystem->getAuthLogin()+"', '"+
                                  authsystem->getAuthPassword() + "',"
                                  +convertToString(authsystem->getUserPasswordEncryption()-1)+ ","
                                  +convertToString(authsystem->getType()-1) +","
                                  +convertToString(authsystem->getStatus()-1)+");");

}

/**
* \brief Function to get the sql code of machines description from a VISHNU configuration
* \fn std::string machineDescToSql(UMS_Data::Machine_ptr machine)
* \param authSystem The authSystem object
* \return the sql code containing the sql code of authentication systems based on ldap
*/
std::string
ConfigurationServer::ldapAuthSystemToSql(UMS_Data::AuthSystem_ptr authsystem) {

  AuthSystemServer authSystemServer(authsystem);

  //If the Ldap base is defined and the type is ldap
  if ((authsystem->getLdapBase().size() != 0)
    //Remove 1 on enum types because of the storage of EMF litteral on file which is shifted of 1
    && ((authsystem->getType()-1) == LDAPTYPE) ) { // LDAP

    std::string numAuth = authSystemServer.getAttribut("where authsystemid='"+authsystem->getAuthSystemId()+"'");
    return (" insert into ldapauthsystem (authsystem_authsystemid, ldapbase) values "
                            "("+numAuth+", '"+authsystem->getLdapBase()+"');");
  }
}