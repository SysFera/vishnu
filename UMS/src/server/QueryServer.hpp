#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

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

  template <class T>
  void addIntegerOptionRequest(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" and "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  void addCondition(const std::string& name, const std::string& value, std::string& request) {
    request.append(" where "+name+"=");
    request.append("'"+value+"'");
  }

  template <class T>
  void addIntegerCondition(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" where "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  long convertToTimeType(std::string date) {
    
    if(date.size()==0) return 0;
   
      boost::posix_time::ptime pt(time_from_string(date));
      boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      time_duration::sec_type time = (pt - epoch).total_seconds();

    return time_t(time);

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
                              where not userid='"+ ROOTUSERNAME +"'";

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

         if(moption.size()!=0) {
           sqlListofUsers.append(" and userid=");
           sqlListofUsers.append("'"+moption+"'");
           //To get the list of users from the database where id=moption
           ListofUsers = mdatabaseVishnu->getResult(sqlListofUsers.c_str());
           if(ListofUsers->getNbTuples()==0) {
             UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
             throw e ;
           }
         } 
         else { 
           //To get the list of users from the database
           ListofUsers = mdatabaseVishnu->getResult(sqlListofUsers.c_str());
        }

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
              user->setStatus(convertToInt(*(++ii)));

              mlistUsers->getUsers().push_back(user);
          }
        }

    } //End if the user is an admin
    else {
          UMSVishnuException e (ERRCODE_NO_ADMIN);
          throw e;
        }
      }//End if the user exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
      catch (VishnuException& e) {
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

  std::string sqlListofMachinesWithJointure = "SELECT machineid, name, site, machine.status, lang, description, userid \
   from machine, description, account, users where machine.nummachineid = description.machine_nummachineid \
   and account.machine_nummachineid=machine.nummachineid and account.users_numuserid=users.numuserid";

   std::string sqlListofMachinesIntial =  sqlListofMachines;

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
      size_t userIdSize = mparameters->getUserId().size();
      size_t machineIdSize = mparameters->getMachineId().size();
      bool isListAll = mparameters->isListAllmachine();
      
      if ((!userServer.isAdmin()) && userIdSize!=0) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
      }

      //IMPORTANT : The order of if test is important
      if(!isListAll) {
         sqlListofMachines = sqlListofMachinesWithJointure;
         addOptionRequest("userid", userServer.getData().getUserId(), sqlListofMachines);
         ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str()); 
      }
      else {  
        //To get the list of machines from the database
        ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());
      }

      //The admin option
      if(userIdSize!=0) {
        
        sqlListofMachines = sqlListofMachinesWithJointure;
        addOptionRequest("userid", mparameters->getUserId(), sqlListofMachines);

        //To get the list of machines from the database where userid = mparameters->getUserId()
        ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());
        if(ListofMachines->getNbTuples()==0) {
           UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
           throw e ;
        }
      }

      if(machineIdSize!=0) {
         if(!isListAll && userIdSize==0) {
           sqlListofMachines=sqlListofMachinesIntial; 
         }
         addOptionRequest("machineid", mparameters->getMachineId(), sqlListofMachines);
         //To get the list of machines from the database where machineid = mparameters->getMachineId()
         ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());

         if(ListofMachines->getNbTuples()==0) {
            UMSVishnuException e(ERRCODE_UNKNOWN_MACHINE);
            throw e ;
         }
      }
     

     if (ListofMachines->getNbTuples() != 0){
       for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
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

    }//End if the user exists
    else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
    catch (VishnuException& e) {
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

    std::string sqlListofLocalAccountInitial = sqlListofLocalAccount;

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

        size_t userIdSize = mparameters->getUserId().size();
        size_t machineIdSize = mparameters->getMachineId().size();
        bool isListAll = mparameters->isAdminListOption();

        if ((!userServer.isAdmin()) && (userIdSize!=0 || isListAll)) {
          UMSVishnuException e (ERRCODE_NO_ADMIN);
          throw e;
        }

        //IMPORTANT : The order of if test is important
        if(!isListAll) {
          addOptionRequest("userid", userServer.getData().getUserId(), sqlListofLocalAccount);
          ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());
        }
        else { // Is an admin option
          //To get the list of machines from the database
          ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());
        }

        //The admin option
        if(userIdSize!=0) {
          sqlListofLocalAccount=sqlListofLocalAccountInitial;
          addOptionRequest("userid", mparameters->getUserId(), sqlListofLocalAccount);

          //To get the list of machines from the database where userid = mparameters->getUserId()
          ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());
          if(ListofLocalAccount->getNbTuples()==0) {
            UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
            throw e ;
          }
        }
     
        if(machineIdSize!=0) {
          if(!isListAll && userIdSize==0) {
            sqlListofLocalAccount=sqlListofLocalAccountInitial;
          }
          addOptionRequest("machineid", mparameters->getMachineId(), sqlListofLocalAccount);
          //To get the list of machines from the database where machineid = mparameters->getMachineId()
          ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());

          if(ListofLocalAccount->getNbTuples()==0) {
            UMSVishnuException e(ERRCODE_UNKNOWN_MACHINE);
            throw e ;
          }
        }
 
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

              mlistObject->getAccounts().push_back(localAccount);
          }
        }
      }//End if the user exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
    catch (VishnuException& e) {
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
    std::string sqlListofOptions = "SELECT description, defaultvalue from optionu";
    std::string sqlListofOptionValues = "SELECT description, value, userid from optionu, optionvalue, users\
                                               where optionu.numoptionid=optionvalue.optionu_numoptionid and\
                                               optionvalue.users_numuserid=users.numuserid";
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

        size_t userIdSize = mparameters->getUserId().size();
        size_t nameSize = mparameters->getOptionName().size();
        bool listAllDefault = mparameters->isListAllDeftValue();

        if ((!userServer.isAdmin()) && (userIdSize!=0)) {
          UMSVishnuException e (ERRCODE_NO_ADMIN);
          throw e;
        }
        
        if(!listAllDefault) {
          
          if(userIdSize==0) {
            sqlListofOptions = sqlListofOptionValues;
            addOptionRequest("userid", userServer.getData().getUserId(), sqlListofOptions);
          } else {
            sqlListofOptions = sqlListofOptionValues;
            addOptionRequest("userid", mparameters->getUserId(), sqlListofOptions);
          }

          if(nameSize!=0) {
            addOptionRequest("description", mparameters->getOptionName(), sqlListofOptions);
          }
        }
        //To get the list of options values from the database
        ListofOptions = mdatabaseVishnu->getResult(sqlListofOptions.c_str());

        if (ListofOptions->getNbTuples() != 0){
          for (size_t i = 0; i < ListofOptions->getNbTuples(); ++i) {
            results.clear();
            results = ListofOptions->get(i);
            ii = results.begin();

              UMS_Data::OptionValue_ptr optionValue = ecoreFactory->createOptionValue();;
              optionValue->setOptionName(*ii);
              optionValue->setValue(*(++ii));
              mlistObject->getOptionValues().push_back(optionValue);
          }
        }
      else {
          UMSVishnuException e (ERRCODE_NO_ADMIN);
          throw e;
        }
      }//End if the user exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
    catch (VishnuException& e) {
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
          for (size_t i = 0; i < ListOfCommands->getNbTuples(); ++i) {
            results.clear();
            results = ListOfCommands->get(i);
            ii = results.begin();

              UMS_Data::Command_ptr command = ecoreFactory->createCommand();
              command->setCommandId(*ii);
              command->setSessionId(*(++ii));
              command->setMachineId(*(++ii));
              command->setCmdDescription(*(++ii));
              //convertToTimeType
              //command->setCmdStartTime(convertToInt(*(++ii))); //TODO: A voir avec Paco
              //command->setCmdEndTime(convertToInt(*(++ii))); //TODO: A voir avec Paco

              mlistObject->getCommands().push_back(command);
          }
        }
      } //End if the user is an admin
      else {
          UMSVishnuException e (ERRCODE_NO_ADMIN);
          throw e;
        }
      }//End if the user exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
    catch (VishnuException& e) {
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

  void processOptions(UserServer userServer, const UMS_Data::ListSessionOptions_ptr& options, std::string& sqlRequest)
  {
   
    size_t userIdSize = options->getUserId().size();
    bool listAll = options->isAdminListOption();

    if ((!userServer.isAdmin()) && (userIdSize!=0 || listAll)) {
       UMSVishnuException e (ERRCODE_NO_ADMIN);
       throw e;
    }
   
    if(options->getMachineId().size()!=0) {
       sqlRequest = "SELECT vsessionid, userid, sessionkey, state, closepolicy, timeout, lastconnect,\
         creation, closure, name from vsession, users, clmachine where vsession.users_numuserid=users.numuserid\
         and vsession.clmachine_numclmachineid=clmachine.numclmachineid";
       addOptionRequest("name", options->getMachineId(), sqlRequest);
    }

    if(userIdSize!=0) {
      addOptionRequest("userid", options->getUserId(), sqlRequest);
    } else {
            if(!listAll) {
               addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest); 
            }
    } 

    int status = options->getStatus();
    addIntegerOptionRequest("state", status, sqlRequest);
    
    if(options->getSessionClosePolicy()) {
       int closePolicy = options->getSessionClosePolicy();
       addIntegerOptionRequest("closepolicy", closePolicy, sqlRequest);
    }

    if(options->getSessionInactivityDelay()) {
      int timeOut = options->getSessionInactivityDelay(); 
      addIntegerOptionRequest("timeout", timeOut, sqlRequest);
    }
   
    if(options->getSessionId().size()!=0) {
      addOptionRequest("vsessionid", options->getSessionId(), sqlRequest);
    }

    if(options->getStartDateOption()!=-1) {
      long startDate = options->getStartDateOption();
      addIntegerOptionRequest("creation", startDate, sqlRequest);
    }
   
    if(options->getEndDateOption()!=-1) {
      long endDate = options->getEndDateOption();
      addIntegerOptionRequest("closure", endDate, sqlRequest);
    }

  }


  //To list sessions
  UMS_Data::ListSessions* list()
  {
    DatabaseResult *ListOfSessions;
    std::string sqlListOfSessions = "SELECT vsessionid, userid, sessionkey, state, closepolicy, timeout, lastconnect, \
    creation, closure from vsession, users where vsession.users_numuserid=users.numuserid";

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

        processOptions(userServer, mparameters, sqlListOfSessions);      
  
        //To get the list of sessions from the database
        ListOfSessions = mdatabaseVishnu->getResult(sqlListOfSessions.c_str());

        if (ListOfSessions->getNbTuples() != 0){
          for (size_t i = 0; i < ListOfSessions->getNbTuples(); ++i) {
            results.clear();
            results = ListOfSessions->get(i);
            ii = results.begin();

              UMS_Data::Session_ptr session = ecoreFactory->createSession();
              session->setSessionId(*(ii));
              session->setUserId(*(++ii));
              session->setSessionKey(*(++ii));
              session->setStatus(convertToInt(*(++ii)));
              session->setClosePolicy(convertToInt(*(++ii)));
              session->setTimeout(convertToInt(*(++ii)));
              session->setDateLastConnect(convertToTimeType(*(++ii))); 
              session->setDateCreation(convertToTimeType(*(++ii))); 
              session->setDateClosure(convertToTimeType(*(++ii))); 

              mlistObject->getSessions().push_back(session);
          }
        }
      }//End if the user exists
      else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
    }
    catch (VishnuException& e) {
        throw;
    }

    return mlistObject;
  }

  //Destructor
  ~QuerySessions() {
  }

};

#endif
