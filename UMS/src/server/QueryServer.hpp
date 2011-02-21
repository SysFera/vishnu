#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionServer.hpp"
#include "ListCmdOptions.hpp"
#include "ListCommands.hpp"
#include "ListLocalAccOptions.hpp"
#include "ListLocalAccounts.hpp"
#include "ListMachineOptions.hpp"
#include "ListMachines.hpp"
#include "ListOptionsValues.hpp"
#include "ListOptOptions.hpp"
#include "ListSessionOptions.hpp"
#include "ListSessions.hpp"
#include "ListUsers.hpp"

template<class QueryParameters, class ListObject> class QueryServer;

//////////////////////////////////////////////////QueryServer Class/////////////////////////////////////////////////
template <class QueryParameters, class ListObject>
class QueryServer
{
public:

  explicit 
  QueryServer(const SessionServer session):msessionServer(session) 
  {
     mlistObject = NULL;
     DbFactory factory;
     mdatabaseVishnu = factory.getDatabaseInstance(); 
  }

  QueryServer(QueryParameters *params, const SessionServer& session):
     mparameters(params), msessionServer(session) 
  {
  
     mlistObject = NULL;
     DbFactory factory;
     mdatabaseVishnu = factory.getDatabaseInstance();
  };

  virtual ListObject* list() = 0;

  void addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
     request.append(" and "+name+"=");
     request.append("'"+value+"'");
  }

  void addCondition(const std::string& name, const std::string& value, std::string& request) {
     request.append(" where "+name+"=");
     request.append("'"+value+"'");
  }

  virtual ~QueryServer() 
  {
  }

protected:

  QueryParameters *mparameters;
  SessionServer msessionServer;
  ListObject* mlistObject;

  /**
   * \brief An instance of vishnu database
   */
  Database *mdatabaseVishnu;


};

//////////////////////////////////////////////////QueryUsers Class/////////////////////////////////////////////////
//QueryUsers class
class QueryUsers 
{

public:
  //Constructors
  explicit QueryUsers(const SessionServer session):msessionServer(session)
  {
     mlistUsers = NULL;
     DbFactory factory;
     mdatabaseVishnu = factory.getDatabaseInstance();
  }
  
  QueryUsers(const std::string& option, const SessionServer& session):
   moption(option), msessionServer(session) 
  {
     mlistUsers = NULL;
     DbFactory factory;
     mdatabaseVishnu = factory.getDatabaseInstance();
  }
 
  //To list users 
  UMS_Data::ListUsers* list() {
  
  DatabaseResult *ListofUsers;  
  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users \
                              where not userid='vishnu_user' and not userid='vishnu_db_admin'";

  if(moption.size()!=0) {
     sqlListofUsers.append(" and userid=");
     sqlListofUsers.append("'"+moption+"'");
  }
  
  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mlistUsers = ecoreFactory->createListUsers();

  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists 
    if (userServer.exist()) {
      //if the user is an admin
      if (userServer.isAdmin()) {

        //To get the list of users from the database
        ListofUsers = mdatabaseVishnu->getResult(sqlListofUsers.c_str());

        if (ListofUsers->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListofUsers->getNbTuples()); ++i) {
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
               user->setStatus(convertToInt(*(++ii)));

               mlistUsers->getUsers().push_back(user); 
           }
        }

    } //End if the user is an admin
     else {
         UMSVishnuException e (4, "The user is not an admin");
          throw e;
        }
      }//End if the user exists
      else {
        UMSVishnuException e (4, "The user is unknown");
        throw e;
       }
     }
      catch (SystemException& e) {
        throw;
      }
 
     return mlistUsers; 
   }

  //Destructor 
  ~QueryUsers() {
   }

  private:

  std::string moption;
  SessionServer msessionServer;
  ListUsers* mlistUsers;

  /**
   * \brief An instance of vishnu database
   */
  Database *mdatabaseVishnu;

};

//////////////////////////////////////////////////QueryMachines Class/////////////////////////////////////////////////
//QueryMachines class
class QueryMachines: public QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>
{

public:
  //Constructors
  explicit QueryMachines(const SessionServer session):QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(session)
  {
  }
  QueryMachines(UMS_Data::ListMachineOptions_ptr params, const SessionServer& session):
   QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(params, session) 
  {
  }

  //To list machines
  UMS_Data::ListMachines* list()
  {

  DatabaseResult *ListofMachines;
  std::string sqlListofMachines = "SELECT machineid, name, site, status, lang, description from machine, description \
   where machine.nummachineid = description.machine_nummachineid";

  //IMPORTANT : le test de UserId doit apparaître avant le test de machineId
  if((mparameters->getUserId()).size()!=0) {

    sqlListofMachines =   "SELECT machineid, name, site, machine.status, lang, description, userid from machine, description, account, users \
    where machine.nummachineid = description.machine_nummachineid and account.machine_nummachineid=machine.nummachineid and \
    account.users_numuserid=users.numuserid";
      addOptionRequest("userid", mparameters->getUserId(), sqlListofMachines); 
  }
  if((mparameters->getMachineId()).size()!=0) {
      addOptionRequest("machineid", mparameters->getMachineId(), sqlListofMachines);
  }
  if(mparameters->isListAllmachine()) {
    //TODO
  }

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mlistObject = ecoreFactory->createListMachines();

  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists 
    if (userServer.exist()) {
      //if the user is an admin
      if (userServer.isAdmin()) {

        //To get the list of machines from the database
        ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());

        if (ListofMachines->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListofMachines->getNbTuples()); ++i) {
            results.clear();
            results = ListofMachines->get(i);
            ii = results.begin();
                UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
                machine->setMachineId(*ii);
                machine->setName(*(++ii));
                machine->setSite(*(++ii));
                machine->setStatus(convertToInt(*(++ii)));
                machine->setLanguage(*(++ii));
                machine->setMachineDescription(*(++ii));

                mlistObject->getMachines().push_back(machine);
            }
        }
 

    } //End if the user is an admin
      else {
        UMSVishnuException e (4, "The user is not an admin");
        throw e;
      }
    }//End if the user exists
     else {
       UMSVishnuException e (4, "The user is unknown");
        throw e;
      }
    }
    catch (SystemException& e) {
      throw;
    } 

    return mlistObject;
  } 

  //Destructor 
  ~QueryMachines() {
   }
};

//////////////////////////////////////////////////QueryLocalAccounts Class/////////////////////////////////////////////////
//QueryLocalAccounts class
class QueryLocalAccounts: public QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>
{

public:
  //Constructors
  QueryLocalAccounts(const SessionServer session):QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(session)
  {
  }
  QueryLocalAccounts(UMS_Data::ListLocalAccOptions_ptr params, const SessionServer& session):
   QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(params, session) 
  {
  }

  //To list LocalAccounts 
  UMS_Data::ListLocalAccounts* list()
  {
     DatabaseResult *ListofLocalAccount;
     std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home \
     from account, machine, users where account.machine_nummachineid=machine.nummachineid and \
     account.users_numuserid=users.numuserid";

    if((mparameters->getUserId()).size()!=0) {
      addOptionRequest("userid", mparameters->getUserId(), sqlListofLocalAccount);
    }
    if((mparameters->getMachineId()).size()!=0) {
      addOptionRequest("machineid", mparameters->getMachineId(), sqlListofLocalAccount);
    }
    if(mparameters->isAdminListOption()) {
    //TODO
    }


     std::vector<std::string>::iterator ii;
     std::vector<std::string> results;
     UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
     mlistObject = ecoreFactory->createListLocalAccounts();

     try {
       //Creation of the object user
       UserServer userServer = UserServer(msessionServer);
       userServer.init();
       //if the user exists 
       if (userServer.exist()) {
      //if the user is an admin
       if (userServer.isAdmin()) {

        //To get the list of local accounts from the database
        ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());

        if (ListofLocalAccount->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListofLocalAccount->getNbTuples()); ++i) {
            results.clear();
            results = ListofLocalAccount->get(i);
            ii = results.begin();
            
              UMS_Data::LocalAccount_ptr localAccount = ecoreFactory->createLocalAccount();
              localAccount->setMachineId(*ii);
              localAccount->setUserId(*(++ii));
              localAccount->setAcLogin(*(++ii));
              localAccount->setSshKeyPath(*(++ii));
              localAccount->setHomeDirectory(*(++ii));
                
              mlistObject->getAccounts().push_back(localAccount);
          }
        }
      } //End if the user is an admin
       else {
           UMSVishnuException e (4, "The user is not an admin");
           throw e;
         }
      }//End if the user exists
      else {
         UMSVishnuException e (4, "The user is unknown");
         throw e;
       }
     }
     catch (SystemException& e) {
        throw;
     }

     return mlistObject;
   }

   //Destructor 
  ~QueryLocalAccounts() {
   }

};

//////////////////////////////////////////////////QueryOptions Class/////////////////////////////////////////////////
//QueryOptions class
class QueryOptions: public QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>
{

public:
  //Constructors
  QueryOptions(const SessionServer session):QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>(session)
  {
  }
  QueryOptions(UMS_Data::ListOptOptions_ptr params, const SessionServer& session):
   QueryServer<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues>(params, session) 
  {
  }

  //To list Options Values 
  UMS_Data::ListOptionsValues* list()
  {
     DatabaseResult *ListofOptions;
     //TODO : A COMPLETER Difference users et admin + option requeêtre correcte à faire
     std::string sqlListofOptions = "SELECT description, defaultvalue from optionu";

     if((mparameters->getUserId()).size()!=0) {
       //TODO
     }
     if((mparameters->getOptionName()).size()!=0) {
       addCondition("description", mparameters->getOptionName(), sqlListofOptions);
     }
     if(mparameters->isListAllDeftValue()) {
      //TODO
     }

     std::vector<std::string>::iterator ii;
     std::vector<std::string> results;
     UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
     mlistObject = ecoreFactory->createListOptionsValues(); 

     try {
       //Creation of the object user
       UserServer userServer = UserServer(msessionServer);
       userServer.init();
       //if the user exists 
       if (userServer.exist()) {
      //if the user is an admin
       if (userServer.isAdmin()) {

        //To get the list of options values from the database
        ListofOptions = mdatabaseVishnu->getResult(sqlListofOptions.c_str());

        if (ListofOptions->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListofOptions->getNbTuples()); ++i) {
            results.clear();
            results = ListofOptions->get(i);
            ii = results.begin();
            
              UMS_Data::OptionValue_ptr optionValue = ecoreFactory->createOptionValue();;
              optionValue->setOptionName(*ii);
              optionValue->setValue(*(++ii));
              mlistObject->getOptionValues().push_back(optionValue);
           }
        }
      } //End if the user is an admin
       else {
           UMSVishnuException e (4, "The user is not an admin");
           throw e;
         }
      }//End if the user exists
      else {
         UMSVishnuException e (4, "The user is unknown");
         throw e;
       }
     }
     catch (SystemException& e) {
        throw;
     }

     return mlistObject;
   }

   //Destructor 
  ~QueryOptions() {
   }

};

//////////////////////////////////////////////////QueryCommands Class/////////////////////////////////////////////////

//QueryCommands class
class QueryCommands: public QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>
{

public:
  //Constructors
  QueryCommands(const SessionServer session):QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(session)
  {
  }
  QueryCommands(UMS_Data::ListCmdOptions_ptr params, const SessionServer& session):
   QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(params, session) 
  {
  }

  //To list commands 
  UMS_Data::ListCommands* list()
  {
     DatabaseResult *ListOfCommands;
     //TODO : A COMPLETER sql à faire!!!
     std::string sqlListOfCommands = "SELECT TODO";

     std::vector<std::string>::iterator ii;
     std::vector<std::string> results;
     UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
     mlistObject = ecoreFactory->createListCommands(); 

     try {
       //Creation of the object user
       UserServer userServer = UserServer(msessionServer);
       userServer.init();
       //if the user exists 
       if (userServer.exist()) {
      //if the user is an admin
       if (userServer.isAdmin()) {

        //To get the list of commands from the database
        ListOfCommands = mdatabaseVishnu->getResult(sqlListOfCommands.c_str());

        if (ListOfCommands->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListOfCommands->getNbTuples()); ++i) {
            results.clear();
            results = ListOfCommands->get(i);
            ii = results.begin();
           
              UMS_Data::Command_ptr command = ecoreFactory->createCommand();
              command->setCommandId(*ii);
              command->setSessionId(*(++ii));
              command->setMachineId(*(++ii));
              command->setCmdDescription(*(++ii));
              //command->setCmdStartTime(convertToInt(*(++ii))); //TODO: A voir avec Paco
              //command->setCmdEndTime(convertToInt(*(++ii))); //TODO: A voir avec Paco
           
              mlistObject->getCommands().push_back(command);
           }
        }
      } //End if the user is an admin
       else {
           UMSVishnuException e (4, "The user is not an admin");
           throw e;
         }
      }//End if the user exists
      else {
         UMSVishnuException e (4, "The user is unknown");
         throw e;
       }
     }
     catch (SystemException& e) {
        throw;
     }

     return mlistObject;
   }

   //Destructor 
  ~QueryCommands() {
   }

};


//////////////////////////////////////////////////QuerySession Class/////////////////////////////////////////////////
//QuerySession class
class QuerySessions: public QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>
{

public:
  //Constructors
  QuerySessions(const SessionServer session):QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(session)
  {
  }
  QuerySessions(UMS_Data::ListSessionOptions_ptr params, const SessionServer& session):
   QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(params, session) 
  {
  }
  
   

  
  //To list sessions 
  UMS_Data::ListSessions* list()
  {
     DatabaseResult *ListOfSessions;
     //TODO : A COMPLETER
     std::string sqlListOfSessions = "SELECT vsessionid, userid, sessionkey, state, closepolicy, timeout \
     from vsession, users where vsession.users_numuserid=users.numuserid";

     std::vector<std::string>::iterator ii;
     std::vector<std::string> results;
     UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
     mlistObject = ecoreFactory->createListSessions(); 

     try {
       //Creation of the object user
       UserServer userServer = UserServer(msessionServer);
       userServer.init();
       //if the user exists 
       if (userServer.exist()) {
      //if the user is an admin
       if (userServer.isAdmin()) {

        //To get the list of sessions from the database
        ListOfSessions = mdatabaseVishnu->getResult(sqlListOfSessions.c_str());

        if (ListOfSessions->getNbTuples() != 0){
          for (int i = 0; i < static_cast <int> (ListOfSessions->getNbTuples()); ++i) {
            results.clear();
            results = ListOfSessions->get(i);
            ii = results.begin();
           
              UMS_Data::Session_ptr session = ecoreFactory->createSession();
              session->setSessionId(*(ii));
              session->setUserId(*(++ii));
              session->setSessionKey(*(++ii));
              /*session->setDateLastConnect(1297275819); //TODO: A voir avec Paco
              session->setDateCreation(1297276819); //TODO: A voir avec Paco
              session->setDateClosure(1297277819);*/ // TODO:A voir avec Paco
              session->setStatus(convertToInt(*(++ii)));
              session->setClosePolicy(convertToInt(*(++ii)));
              session->setTimeout(convertToInt(*(++ii)));

              mlistObject->getSessions().push_back(session);
           }
        }
      } //End if the user is an admin
       else {
           UMSVishnuException e (4, "The user is not an admin");
           throw e;
         }
      }//End if the user exists
      else {
         UMSVishnuException e (4, "The user is unknown");
         throw e;
       }
     }
     catch (SystemException& e) {
        throw;
     }

     return mlistObject;
   }

   //Destructor 
  ~QuerySessions() {
   }

};

#endif
