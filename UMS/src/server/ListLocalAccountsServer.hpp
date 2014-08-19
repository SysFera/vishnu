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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

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
   * \brief Constructor, raises an exception on error
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   */
  ListLocalAccountsServer(const SessionServer session)
    : QueryServer<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts>(),
      mcommandName("vishnu_list_local_accounts"),
      msessionServer(session)
  {

  }

  /**
   * \brief Function to treat the ListLocalAccountsServer options
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListLocalAccountsServer options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListLocalAccOptions_ptr& options, std::string& sqlRequest)
  {
    std::string sqlListofLocalAccountInitial = sqlRequest;

    bool isListAll = options->isAdminListOption();

    if (! userServer.isAdmin()
        && (! options->getUserId().empty() || isListAll)) {
      throw UMSVishnuException(ERRCODE_NO_ADMIN);
    }

    if(! isListAll) {
      addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
    }

    //The admin option
    if (! options->getUserId().empty()) {
      sqlRequest = sqlListofLocalAccountInitial;
      std::string numUser = getNumUser(options->getUserId());
      addOptionRequest("users.numuserid", numUser, sqlRequest);
    }

    if (! options->getMachineId().empty()) {

      std::string numMachine = getNumMachine(options->getMachineId());

      if (! isListAll && options->getUserId().empty()) {
        sqlRequest = sqlListofLocalAccountInitial;
        addOptionRequest("users.userid", userServer.getData().getUserId(), sqlRequest);
      }

      addOptionRequest("machine.nummachineid", numMachine, sqlRequest);
    }
  }

  /**
   * \brief Function to list locoal accounts information
   * \return The pointer to the UMS_Data::ListLocalAccounts containing local accounts information
   * \return raises an exception on error
   */
  UMS_Data::ListLocalAccounts* list(UMS_Data::ListLocalAccOptions_ptr option)
  {
    std::string query = boost::str(boost::format("SELECT DISTINCT machineid, userid, aclogin, home"
                                                 " FROM account, machine, users"
                                                 " WHERE account.machine_nummachineid = machine.nummachineid"
                                                 "   AND account.users_numuserid      = users.numuserid"
                                                 "   AND users.status                 =%1%"
                                                 "   AND machine.status               =%1%"
                                                 "   AND account.status               =%1%"
                                                 ) % vishnu::STATUS_ACTIVE);


    std::vector<std::string>::iterator dbResultIter;
    std::vector<std::string> dbResults;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListLocalAccounts();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    if (! userServer.exist()) {
      throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
    }

    //To process options
    processOptions(userServer, option, query);

    boost::scoped_ptr<DatabaseResult> resultAccount(mdatabase->getResult(query));
    if (resultAccount->getNbTuples() != 0){
      for (size_t i = 0; i < resultAccount->getNbTuples(); ++i) {
        dbResults.clear();
        dbResults = resultAccount->get(i);
        dbResultIter = dbResults.begin();

        UMS_Data::LocalAccount_ptr localAccount = ecoreFactory->createLocalAccount();
        localAccount->setMachineId(*dbResultIter);
        localAccount->setUserId(*(++dbResultIter));
        localAccount->setAcLogin(*(++dbResultIter));
        localAccount->setHomeDirectory(*(++dbResultIter));

        mlistObject->getAccounts().push_back(localAccount);
      }
    }

    return mlistObject;
  }

  /**
   * \brief Function to get the name of the ListLocalAccountsServer command line
   * \return The the name of the ListLocalAccountsServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListLocalAccountsServer()
  {
  }

private:
  /**
  * \brief The name of the ListLocalAccountsServer command line
  */
  std::string mcommandName;

  /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;
};

#endif
