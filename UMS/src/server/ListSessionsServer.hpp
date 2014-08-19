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
   * \brief Constructor, raises an exception on error
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   */
  ListSessionsServer(const SessionServer session)
    : QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(),
      mcommandName("vishnu_list_sessions"),
      msessionServer(session)
  {
  }

  /**
   * \brief Function to treat the listSessionServer options
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListSessionServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(UserServer userServer, const UMS_Data::ListSessionOptions_ptr& options, std::string& query)
  {
    boost::posix_time::ptime pt;
    bool listAll = options->isAdminListOption();

    if (! userServer.isAdmin()
        && (! options->getUserId().empty() || listAll)) {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }

    if(! options->getMachineId().empty()) {
      addOptionRequest("clmachine.name", options->getMachineId(), query);
    }

    if(! options->getUserId().empty()) {
      addOptionRequest("users.userid", options->getUserId(), query);
    } else {
      if (! listAll) {
        addOptionRequest("users.userid", userServer.getData().getUserId(), query);
      }
    }

    int status = options->getStatus();
    if (status == vishnu::STATUS_UNDEFINED) {
      status = vishnu::STATUS_ACTIVE;
    }

    addIntegerOptionRequest("state", status, query);

    if (options->getSessionClosePolicy()) {
      addIntegerOptionRequest("closepolicy", options->getSessionClosePolicy(), query);
    }

    if (options->getSessionInactivityDelay() < 0) {
      throw UMSVishnuException(ERRCODE_INCORRECT_TIMEOUT);
    }

    if (options->getSessionInactivityDelay()) {
      addIntegerOptionRequest("timeout", options->getSessionInactivityDelay(), query);
    }

    if (! options->getSessionId().empty()) {
      addOptionRequest("vsessionid", options->getSessionId(), query);
    }

    time_t startDate = static_cast<time_t>(options->getStartDateOption());
    if (startDate > 0) {
      addTimeRequest("creation", vishnu::timeToTimestamp(startDate), query, ">=");
    }

    time_t endDate = static_cast<time_t>(options->getEndDateOption());
    if (endDate > 0) {
      addTimeRequest("closure", vishnu::timeToTimestamp(endDate), query, "<=");
    }
  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  UMS_Data::ListSessions*
  list(UMS_Data::ListSessionOptions_ptr option)
  {
    std::string query = (boost::format("SELECT DISTINCT vsessionid, userid, sessionkey, state, closepolicy, "
                                       "                timeout, lastconnect, creation, closure, authid "
                                       " FROM vsession, users, clmachine"
                                       " WHERE vsession.state            = %1%"
                                       "    AND users.status             = %1%"
                                       "    AND vsession.users_numuserid = users.numuserid"
                                       "    AND vsession.clmachine_numclmachineid=clmachine.numclmachineid")
                         % vishnu::STATUS_ACTIVE).str();

    std::vector<std::string>::iterator dbResultIter;
    std::vector<std::string> dbResults;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListSessions();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();

    if (! userServer.exist()) {
      throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
    }

    processOptions(userServer, option, query);
    query.append(" order by creation");
    //To get the list of sessions from the database
    boost::scoped_ptr<DatabaseResult> ListOfSessions (mdatabase->getResult(query));

    if (ListOfSessions->getNbTuples() != 0){
      for (size_t i = 0; i < ListOfSessions->getNbTuples(); ++i) {
        dbResults.clear();
        dbResults = ListOfSessions->get(i);
        dbResultIter = dbResults.begin();

        UMS_Data::Session_ptr session = ecoreFactory->createSession();
        session->setSessionId(*(dbResultIter));
        session->setUserId(*(++dbResultIter));
        session->setSessionKey(*(++dbResultIter));
        session->setStatus(vishnu::convertToInt(*(++dbResultIter)));
        session->setClosePolicy(vishnu::convertToInt(*(++dbResultIter)));
        session->setTimeout(vishnu::convertToInt(*(++dbResultIter)));
        session->setDateLastConnect(convertToTimeType(*(++dbResultIter)));
        session->setDateCreation(convertToTimeType(*(++dbResultIter)));
        session->setDateClosure(convertToTimeType(*(++dbResultIter)));
        session->setAuthenId(*(++dbResultIter));
        mlistObject->getSessions().push_back(session);
      }
    }

    return mlistObject;
  }


  /**
   * \brief Function to get the name of the ListSessionsServer command line
   * \return The the name of the ListSessionsServer command line
   */
  std::string
  getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListSessionsServer()
  {
  }

private:

  /**
  * \brief The name of the ListSessionsServer command line
  */
  std::string mcommandName;

  /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;
};

#endif
