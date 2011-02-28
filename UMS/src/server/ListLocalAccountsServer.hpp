#ifndef _LIST_LOCAL_ACCOUNTS
#define _LIST_LOCAL_ACCOUNTS

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListLocalAccOptions.hpp"
#include "ListLocalAccounts.hpp"

class ListLocalAccountsServer: public QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>
{

public:
  //Constructors
  ListLocalAccountsServer(const SessionServer session):
    QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(session)
  {
    mcommandName = "vishnu_list_local_account";
  }
  ListLocalAccountsServer(UMS_Data::ListLocalAccOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(params, session)
  {
    mcommandName = "vishnu_list_local_account";
  }

  //To process options
  void processOptions(UserServer userServer, const UMS_Data::ListLocalAccOptions_ptr& options, std::string& sqlRequest)
  {
     std::string sqlListofLocalAccountInitial = sqlRequest;

     size_t userIdSize = options->getUserId().size();
     size_t machineIdSize = options->getMachineId().size();
     bool isListAll = options->isAdminListOption();

     if ((!userServer.isAdmin()) && (userIdSize!=0 || isListAll)) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
     }

     if(!isListAll) {
        addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
     }
     //The admin option
     if(userIdSize!=0) {
        sqlRequest=sqlListofLocalAccountInitial;
        addOptionRequest("userid", options->getUserId(), sqlRequest);

        DatabaseResult *ListofLocalAccount = mdatabaseVishnu->getResult(sqlRequest.c_str());
        if(ListofLocalAccount->getNbTuples()==0) {
          UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
          throw e ;
        }
     }

     if(machineIdSize!=0) {
        if(!isListAll && userIdSize==0) {
          sqlRequest=sqlListofLocalAccountInitial;
        }
        addOptionRequest("machineid", options->getMachineId(), sqlRequest);
        
        DatabaseResult *ListofLocalAccount = mdatabaseVishnu->getResult(sqlRequest.c_str());
        if(ListofLocalAccount->getNbTuples()==0) {
           UMSVishnuException e(ERRCODE_UNKNOWN_MACHINE);
           throw e ;
         }
     }

  }

  //To list sessions
  UMS_Data::ListLocalAccounts* list()
  {
    DatabaseResult *ListofLocalAccount;
    std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home \
    from account, machine, users where account.machine_nummachineid=machine.nummachineid and \
    account.users_numuserid=users.numuserid";

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
          
          //To process options 
          processOptions(userServer, mparameters, sqlListofLocalAccount);

          ListofLocalAccount = mdatabaseVishnu->getResult(sqlListofLocalAccount.c_str());
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
        } else {
          UMSVishnuException e (ERRCODE_UNKNOWN_USER);
          throw e;
        }
    }
    catch (VishnuException& e) {
        throw;
    }

    return mlistObject;
  }

  std::string getCommandName()
  {
    return mcommandName;
  }

  //Destructor
  ~ListLocalAccountsServer() 
  {
  }

  private:

  std::string mcommandName;
};

#endif
