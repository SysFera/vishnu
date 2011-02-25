#ifndef _LIST_COMMANDS_SERVER
#define _LIST_COMMANDS_SERVER

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListCmdOptions.hpp"
#include "ListCommandsServer.hpp"

class ListCommandsServer: public QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>
{

public:
  //Constructors
  ListCommandsServer(const SessionServer session):
    QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(session)
  {
  }
  ListCommandsServer(UMS_Data::ListCmdOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(params, session)
  {
  }

  //To process options
  void processOptions(UserServer userServer, const UMS_Data::ListCmdOptions_ptr& options, std::string& sqlRequest) 
  {
    boost::posix_time::ptime pt;
    size_t userIdSize = options->getUserId().size();
    bool listAll = options->isAdminListOption();

    if ((!userServer.isAdmin()) && (userIdSize!=0 || listAll)) {
       UMSVishnuException e (ERRCODE_NO_ADMIN);
       throw e;
    }

    if(userIdSize!=0) {
      addOptionRequest("userid", options->getUserId(), sqlRequest);
    } else {
            if(!listAll) {
               addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
            }
    }

    if(options->getSessionId().size()!=0) {
      addOptionRequest("vsessionid", options->getSessionId(), sqlRequest);
    }

    long startDate = options->getStartDateOption();
    if(startDate!=-1) {
       pt =  boost::posix_time::from_time_t(startDate);
       std::string startDateStr =  boost::posix_time::to_simple_string(pt);
       addOptionRequest("starttime", startDateStr, sqlRequest);
    }

    long endDate = options->getEndDateOption();
    if(endDate!=-1) {
       pt =  boost::posix_time::from_time_t(endDate);
       std::string endDateStr =  boost::posix_time::to_simple_string(pt);
       addOptionRequest("endtime", endDateStr, sqlRequest);
    }

  }

  //To get the list of commands 
  void getListOfCommands(UserServer userServer, UMS_Data::ListCommands_ptr& listObject)
  {
    DatabaseResult *ListOfCommands;
    std::string sqlListOfCommands;
    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;

    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();

    sqlListOfCommands = "SELECT ctype, vsessionid, name, description, starttime, endtime, userid from\
                         vsession, clmachine, command, users where command.vsession_numsessionid=vsession.numsessionid and\
                         vsession.clmachine_numclmachineid=clmachine.numclmachineid and  vsession.users_numuserid=users.numuserid";

     processOptions(userServer, mparameters, sqlListOfCommands);

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
           command->setCmdStartTime(convertToTimeType(*(++ii)));
           command->setCmdEndTime(convertToTimeType(*(++ii)));

           listObject->getCommands().push_back(command);
          }
        }

  }

  //To return the list of commands 
  UMS_Data::ListCommands* list()
  {
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListCommands();

    try {
      //Creation of the object user
      UserServer userServer = UserServer(msessionServer);
      userServer.init();
      //if the user exists
      if (userServer.exist()) {

         //get list commands
          getListOfCommands(userServer, mlistObject);

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
  ~ListCommandsServer() {
  }

};

#endif
