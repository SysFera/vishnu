/**
 * \file ListAuthSystemsServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2012
 */
#ifndef _LIST_AUTH_SYSTEMS_SERVER_
#define _LIST_AUTH_SYSTEMS_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "QueryServer.hpp"

#include "ListAuthSystems.hpp"
#include "ListAuthSysOptions.hpp"

/**
 * \class ListAuthSystemsServer
 * \brief ListAuthSystemsServer class implementation
 */
class ListAuthSystemsServer: public QueryServer<UMS_Data::ListAuthSysOptions, UMS_Data::ListAuthSystems>
{

public:

  /**
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListAuthSystemsServer(const SessionServer& session)
    : QueryServer<UMS_Data::ListAuthSysOptions, UMS_Data::ListAuthSystems>(),
      mcommandName("vishnu_list_auth_systems"),
      msessionServer(session)
  {

  }

  /**
   * \brief Function to treat the ListAuthSystemsServer options
   * \param userServer the object which encapsulates user information
   * \param options The object which encapsulates the information of ListAuthSystemsServer  options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(UserServer userServer, const UMS_Data::ListAuthSysOptions_ptr& options, std::string& sqlRequest)
  {
    mfullInfo = options->isListFullInfo();
    if (mfullInfo) {
      if (! userServer.isAdmin()) {
        throw UMSVishnuException(ERRCODE_NO_ADMIN);
      }
    }

    std::string userIdFilter = "";
    if (! options) {
      userIdFilter = userServer.getData().getUserId();
    } else {
      std::string  userId = options->getUserId();
      if (! userId.empty()) {
        if (! userServer.isAdmin()) {
          throw UMSVishnuException(ERRCODE_NO_ADMIN);
        }
        userIdFilter = userId;
      }

      std::string  authSystemId = options->getAuthSystemId();
      if (! authSystemId.empty()) {
        checkAuthSystemId(authSystemId);
        addOptionRequest("numauthsystemid", authSystemId, sqlRequest);
      }

      //If the option for listing all authSystem has not defined
      if (! options->isListAllAuthSystems()) {
        if (userId.empty() && authSystemId.empty()) {
          userIdFilter = userServer.getData().getUserId();
        }
      }
    }

    if (! userIdFilter.empty()) {
      std::string subQuery = (boost::format("SELECT DISTINCT numauthsystemid"
                                            "       FROM authsystem, users, authaccount"
                                            "       WHERE users.status != '%1%'"
                                            "        AND users.userid  = '%2%'"
                                            "        AND authaccount.authsystem_numauthsystemid = authsystem.numauthsystemid"
                                            "        AND authaccount.users_numuserid = users.numuserid")
                              % vishnu::STATUS_DELETED
                              % mdatabase->escapeData(userIdFilter)
                              ).str();

      sqlRequest.append(" AND numauthsystemid IN ("+subQuery+")");
    }
  }

  /**
  * \brief Function to list machines information
  * \return The pointer to the UMS_Data::ListAuthSystems containing users information
  * \return raises an exception on error
  */
  UMS_Data::ListAuthSystems* list(UMS_Data::ListAuthSysOptions_ptr option) {

    std::string sql = (boost::format("SELECT DISTINCT numauthsystemid, name, uri, authlogin, authpassword,"
                                     "       userpwdencryption, authtype, authsystem.status, ldapbase "
                                     " FROM  authsystem, ldapauthsystem"
                                     " WHERE ldapauthsystem.authsystem_numauthsystemid = authsystem.numauthsystemid"
                                     "  AND authsystem.status != %1%")
                       % vishnu::STATUS_DELETED).str();
    std::vector<std::string>::iterator dbResultIter;
    std::vector<std::string> dbResults;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListAuthSystems();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (! userServer.exist()) {
      throw  UMSVishnuException (ERRCODE_UNKNOWN_USER);
    }

    processOptions(userServer, option, sql);
    sql.append(" ORDER BY numauthsystemid");
    //To get the list of authSystems from the database
    boost::scoped_ptr<DatabaseResult> ListofAuthSystems (mdatabase->getResult(sql.c_str()));
    if (ListofAuthSystems->getNbTuples() != 0){
      for (size_t i = 0; i < ListofAuthSystems->getNbTuples(); ++i) {
        dbResults.clear();
        dbResults = ListofAuthSystems->get(i);
        dbResultIter = dbResults.begin();
        UMS_Data::AuthSystem_ptr authSystem = ecoreFactory->createAuthSystem();
        authSystem->setAuthSystemId(*dbResultIter);
        authSystem->setName(*(++dbResultIter));
        authSystem->setURI(*(++dbResultIter));
        std::string authLogin(*(++dbResultIter));
        std::string authPassword(*(++dbResultIter));
        int userPasswordEncryption(vishnu::convertToInt(*(++dbResultIter)));
        int type(vishnu::convertToInt(*(++dbResultIter)));
        if (mfullInfo) {
          authSystem->setAuthLogin(authLogin);
          authSystem->setAuthPassword(authPassword);
          authSystem->setUserPasswordEncryption(userPasswordEncryption);
          authSystem->setType(type);
        }
        authSystem->setStatus(vishnu::convertToInt(*(++dbResultIter)));
        authSystem->setLdapBase(*(++dbResultIter));
        mlistObject->getAuthSystems().push_back(authSystem);
      }
    }
    return mlistObject;
  }

  /**
   * \brief Function to get the name of the ListAuthSystemsServer command line
   * \return The the name of the ListAuthSystemsServer command line
   */
  std::string
  getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListAuthSystemsServer() { }

private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The name of the ListMachinesServer command line
  */
  std::string mcommandName;
  /**
  * \brief An option to have full information
  */
  bool mfullInfo;

  /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;
};

#endif
