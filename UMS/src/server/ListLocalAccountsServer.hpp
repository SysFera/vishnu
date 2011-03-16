/**
 * \file ListLocalAccountsServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
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

/**
 * \class ListLocalAccountsServer
 * \brief ListLocalAccountsServer class implementation
 */
class ListLocalAccountsServer: public QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>
{

public:

  /**
   * \fn ListLocalAccountsServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListLocalAccountsServer(const SessionServer session):
    QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(session)
  {
    mcommandName = "vishnu_list_local_account";
  }
  /**
   * \fn ListLocalAccountsServer(UMS_Data::ListLocalAccOptions_ptr params,
   *                             const SessionServer& session)
   * \param params The object which encapsulates the information of ListLocalAccountsServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListLocalAccountsServer(UMS_Data::ListLocalAccOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(params, session)
  {
    mcommandName = "vishnu_list_local_account";
  }

  /**
   * \brief Function to treat the ListLocalAccountsServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListLocalAccOptions_ptr& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListLocalAccountsServer options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
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
        //To check if the user id is correct
        checkUserId(options->getUserId());

        sqlRequest=sqlListofLocalAccountInitial;
        addOptionRequest("userid", options->getUserId(), sqlRequest);
     }

     if(machineIdSize!=0) {
        //To check if the machine id is correct
        checkMachineId(options->getMachineId());

        if(!isListAll && userIdSize==0) {
          sqlRequest=sqlListofLocalAccountInitial;
          addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
        }
        addOptionRequest("machineid", options->getMachineId(), sqlRequest);
     }

  }

  /**
   * \brief Function to list locoal accounts information
   * \fn UMS_Data::ListLocalAccounts* list()
   * \return The pointer to the UMS_Data::ListLocalAccounts containing local accounts information
   * \return raises an exception on error
   */
  UMS_Data::ListLocalAccounts* list()
  {
    std::string sqlListofLocalAccount = "SELECT machineid, userid, aclogin, sshpathkey, home "
    "from account, machine, users where account.machine_nummachineid=machine.nummachineid and "
    "account.users_numuserid=users.numuserid";

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListLocalAccounts();


    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      //To process options
      processOptions(userServer, mparameters, sqlListofLocalAccount);

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

          mlistObject->getAccounts().push_back(localAccount);
        }
      }
    }
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
    return mlistObject;
  }

  /**
   * \brief Function to get the name of the ListLocalAccountsServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListLocalAccountsServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListLocalAccountsServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListLocalAccountsServer()
  {
  }

  private:

   /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListLocalAccountsServer command line
  */
  std::string mcommandName;

};

#endif
