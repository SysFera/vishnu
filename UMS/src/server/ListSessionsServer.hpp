/**
 * \file ListSessionsServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_SESSIONS_SERVER_
#define _LIST_SESSIONS_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListSessionOptions.hpp"
#include "ListSessions.hpp"

/**
 * \class ListSessionsServer
 * \brief ListSessionsServer class implementation
 */
class ListSessionsServer: public QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>
{

public:

  /**
   * \fn ListSessionsServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListSessionsServer(const SessionServer session):
    QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(session)
  {
   mcommandName = "vishnu_list_sessions";
  }
  /**
   * \fn ListSessionsServer(const UMS_Data::ListSessionOptions_ptr params,
   *                        const SessionServer& session)
   * \param params The object which encapsulates the information of ListSessionsServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListSessionsServer(UMS_Data::ListSessionOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(params, session)
  {
    mcommandName = "vishnu_list_sessions";
  }

  /**
   * \brief Function to treat the listSessionServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListSessionOptions_ptr& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListSessionServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListSessionOptions_ptr& options, std::string& sqlRequest)
  {
     boost::posix_time::ptime pt;
     size_t userIdSize = options->getUserId().size();
     bool listAll = options->isAdminListOption();

     if ((!userServer.isAdmin()) && (userIdSize!=0 || listAll)) {
       UMSVishnuException e (ERRCODE_NO_ADMIN);
       throw e;
     }

     if(options->getMachineId().size()!=0) {
       //To check if the name of the machine is correct
       checkClientMachineName(options->getMachineId());

       sqlRequest = "SELECT vsessionid, userid, sessionkey, state, closepolicy, timeout, lastconnect,"
         "creation, closure, name from vsession, users, clmachine where vsession.users_numuserid=users.numuserid"
         " and vsession.clmachine_numclmachineid=clmachine.numclmachineid";
       addOptionRequest("name", options->getMachineId(), sqlRequest);
     }

     if(userIdSize!=0) {
       //To check if the user id is correct
       checkUserId(options->getUserId());

       addOptionRequest("userid", options->getUserId(), sqlRequest);
     } else {
            if(!listAll) {
               addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
            }
     }

     int status = options->getStatus();
     //To check the status value
     checkStatus(status);

     addIntegerOptionRequest("state", status, sqlRequest);

     int closePolicy = options->getSessionClosePolicy();
     //To check the closePolicy value
     checkClosePolicy(closePolicy);

     if(closePolicy) {
       addIntegerOptionRequest("closepolicy", closePolicy, sqlRequest);
     }

     int timeOut = options->getSessionInactivityDelay();
     if(timeOut < 0) {
       throw UMSVishnuException(ERRCODE_INCORRECT_TIMEOUT);
     }
     if(timeOut) {
       addIntegerOptionRequest("timeout", timeOut, sqlRequest);
     }

     if(options->getSessionId().size()!=0) {
       //To check if the session id is correct
       checkSessionId(options->getSessionId());

       addOptionRequest("vsessionid", options->getSessionId(), sqlRequest);
     }

     time_t startDate = static_cast<time_t>(options->getStartDateOption());
     if(startDate!=-1) {
       pt =  boost::posix_time::from_time_t(startDate);
       std::string startDateStr =  boost::posix_time::to_simple_string(pt);
       addTimeRequest("creation", startDateStr, sqlRequest, ">=");
     }

     time_t endDate = static_cast<time_t>(options->getEndDateOption());
     if(endDate!=-1) {
       pt =  boost::posix_time::from_time_t(endDate);
       std::string endDateStr =  boost::posix_time::to_simple_string(pt);
       addTimeRequest("closure", endDateStr, sqlRequest, "<=");
     }

  }

  /**
   * \brief Function to list sessions information
   * \fn UMS_Data::ListSessions* list()
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  UMS_Data::ListSessions* list()
  {
    std::string sqlListOfSessions = "SELECT vsessionid, userid, sessionkey, state, closepolicy, timeout, lastconnect, "
    "creation, closure from vsession, users where vsession.users_numuserid=users.numuserid";

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListSessions();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      processOptions(userServer, mparameters, sqlListOfSessions);

      //To get the list of sessions from the database
      boost::scoped_ptr<DatabaseResult> ListOfSessions (mdatabaseVishnu->getResult(sqlListOfSessions.c_str()));

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
    }
    else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
    return mlistObject;
  }


  /**
   * \brief Function to get the name of the ListSessionsServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListSessionsServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListSessionsServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListSessionsServer()
  {
  }

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListSessionsServer command line
  */
  std::string mcommandName;

};

#endif
