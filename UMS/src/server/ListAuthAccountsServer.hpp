/**
 * \file ListAuthAccountsServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2012
 */

#ifndef _LIST_AUTH_ACCOUNTS
#define _LIST_AUTH_ACCOUNTS

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "QueryServer.hpp"

#include "ListAuthAccOptions.hpp"
#include "ListAuthAccounts.hpp"

/**
 * \class ListAuthAccountsServer
 * \brief ListAuthAccountsServer class implementation
 */
class ListAuthAccountsServer: public QueryServer<UMS_Data::ListAuthAccOptions, UMS_Data::ListAuthAccounts>
{

public:

  /**
   * \fn ListAuthAccountsServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListAuthAccountsServer(const SessionServer session):
    QueryServer<UMS_Data::ListAuthAccOptions, UMS_Data::ListAuthAccounts>(session)
  {
    mcommandName = "vishnu_list_auth_accounts";
  }
  /**
   * \fn ListAuthAccountsServer(UMS_Data::ListAuthAccOptions_ptr params,
   *                             const SessionServer& session)
   * \param params The object which encapsulates the information of ListAuthAccountsServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListAuthAccountsServer(UMS_Data::ListAuthAccOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListAuthAccOptions, UMS_Data::ListAuthAccounts>(params, session)
  {
    mcommandName = "vishnu_list_auth_accounts";
  }

  /**
   * \brief Function to treat the ListAuthAccountsServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListAuthAccOptions_ptr& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListAuthAccountsServer options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListAuthAccOptions_ptr& options, std::string& sqlRequest)
  {
     std::string sqlListofAuthAccountInitial = sqlRequest;

     size_t userIdSize = options->getUserId().size();
     size_t authSystemIdSize = options->getAuthSystemId().size();
     bool isListAll = options->isListAll();

     if ((!userServer.isAdmin()) && (userIdSize!=0 || isListAll)) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
     }

     //The admin option
     if(userIdSize!=0) {
        //To check if the user id is correct
        checkUserId(options->getUserId());
        sqlRequest=sqlListofAuthAccountInitial;
        addOptionRequest("userid", options->getUserId(), sqlRequest);
     }
     else {
       if(!isListAll) {
        addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
     }
    }

     if(authSystemIdSize!=0) {
        //To check if the machine id is correct
        checkAuthSystemId(options->getAuthSystemId());
        if(!isListAll && userIdSize==0) {
          sqlRequest=sqlListofAuthAccountInitial;
          addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
        }
        addOptionRequest("authsystemid", options->getAuthSystemId(), sqlRequest);
     }

  }

  /**
   * \brief Function to list locoal accounts information
   * \fn UMS_Data::ListAuthAccounts* list()
   * \return The pointer to the UMS_Data::ListAuthAccounts containing auth accounts information
   * \return raises an exception on error
   */
  UMS_Data::ListAuthAccounts* list()
  {
    std::string sqlListofAuthAccount = "SELECT authsystemid, userid, aclogin "
    " from authaccount, authsystem, users where authaccount.authsystem_authsystemid=authsystem.numauthsystemid and authaccount.users_numuserid=users.numuserid";

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListAuthAccounts();


    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      //To process options
      processOptions(userServer, mparameters, sqlListofAuthAccount);

      boost::scoped_ptr<DatabaseResult> ListofAuthAccount (mdatabaseVishnu->getResult(sqlListofAuthAccount.c_str()));
      if (ListofAuthAccount->getNbTuples() != 0){
        for (size_t i = 0; i < ListofAuthAccount->getNbTuples(); ++i) {
          results.clear();
          results = ListofAuthAccount->get(i);
          ii = results.begin();

          UMS_Data::AuthAccount_ptr authAccount = ecoreFactory->createAuthAccount();
          authAccount->setAuthSystemId(*ii);
          authAccount->setUserId(*(++ii));
          authAccount->setAcLogin(*(++ii));
          mlistObject->getAuthAccounts().push_back(authAccount);
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
   * \brief Function to get the name of the ListAuthAccountsServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListAuthAccountsServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListAuthAccountsServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListAuthAccountsServer()
  {
  }

  private:

   /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListAuthAccountsServer command line
  */
  std::string mcommandName;

};

#endif
