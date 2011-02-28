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


//QuerySession class
class ListSessionsServer: public QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>
{

public:
  //Constructors
  ListSessionsServer(const SessionServer session):
    QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(session)
  {
   mcommandName = "vishnu_list_sessions";
  }
  ListSessionsServer(UMS_Data::ListSessionOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListSessionOptions, UMS_Data::ListSessions>(params, session)
  {
    mcommandName = "vishnu_list_sessions";
  }

  //To process options
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

    int closePolicy = options->getSessionClosePolicy();
    if(closePolicy) {
       addIntegerOptionRequest("closepolicy", closePolicy, sqlRequest);
    }

    int timeOut = options->getSessionInactivityDelay();
    if(timeOut) {
      addIntegerOptionRequest("timeout", timeOut, sqlRequest);
    }

    if(options->getSessionId().size()!=0) {
      addOptionRequest("vsessionid", options->getSessionId(), sqlRequest);
    }

    long startDate = options->getStartDateOption();
    if(startDate!=-1) {
      pt =  boost::posix_time::from_time_t(startDate);
      std::string startDateStr =  boost::posix_time::to_simple_string(pt);
      addOptionRequest("creation", startDateStr, sqlRequest);
    }

    long endDate = options->getEndDateOption();
    if(endDate!=-1) {
      pt =  boost::posix_time::from_time_t(endDate);
      std::string endDateStr =  boost::posix_time::to_simple_string(pt);
      addIntegerOptionRequest("closure", endDateStr, sqlRequest);
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

  std::string getCommandName()
  {
    return mcommandName;
  }

  //Destructor
  ~ListSessionsServer() 
  {
  }
 
  private:

  std::string mcommandName;
};

#endif
