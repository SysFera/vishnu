#ifndef _LIST_MACHINES_SERVER_
#define _LIST_MACHINES_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "QueryServer.hpp"
#include "ListMachineOptions.hpp"
#include "ListMachines.hpp"

class ListMachinesServer: public QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>
{

public:
  //Constructors
  ListMachinesServer(const SessionServer session):
  QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(session)
  {
    mcommandName = "vishnu_list_machine";
  }
  ListMachinesServer(UMS_Data::ListMachineOptions_ptr params, const SessionServer& session):
  QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(params, session)
  {
    mcommandName = "vishnu_list_machine";
  }

  //To process options
  void processOptions(UserServer userServer, const UMS_Data::ListMachineOptions_ptr& options, std::string& sqlRequest) 
  {
    std::string sqlListofMachinesWithJointure = "SELECT machineid, name, site, machine.status, lang, description, userid \
    from machine, description, account, users where machine.nummachineid = description.machine_nummachineid \
    and account.machine_nummachineid=machine.nummachineid and account.users_numuserid=users.numuserid";

    std::string sqlListofMachinesIntial =  sqlRequest;

    size_t userIdSize = options->getUserId().size();
    size_t machineIdSize = options->getMachineId().size();
    bool isListAll = options->isListAllmachine();

    if ((!userServer.isAdmin()) && userIdSize!=0) {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }

    if(!isListAll) {
       sqlRequest = sqlListofMachinesWithJointure;
       addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
    }

    //The admin option
    if(userIdSize!=0) {
      sqlRequest = sqlListofMachinesWithJointure;
      addOptionRequest("userid", options->getUserId(), sqlRequest);

      DatabaseResult *ListofMachines = mdatabaseVishnu->getResult(sqlRequest.c_str());
      if(ListofMachines->getNbTuples()==0) {
         UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
         throw e ;
      }
    }

    if(machineIdSize!=0) {
      if(!isListAll && userIdSize==0) {
         sqlRequest=sqlListofMachinesIntial;
      }
      addOptionRequest("machineid", options->getMachineId(), sqlRequest);
      
      DatabaseResult *ListofMachines = mdatabaseVishnu->getResult(sqlRequest.c_str());
      if(ListofMachines->getNbTuples()==0) {
        UMSVishnuException e(ERRCODE_UNKNOWN_MACHINE);
        throw e ;
      }
    }
 
 }

 //To list sessions
 UMS_Data::ListMachines* list()
 {
    DatabaseResult *ListofMachines;
    std::string sqlListofMachines = "SELECT machineid, name, site, status, lang, description from machine, description \
    where machine.nummachineid = description.machine_nummachineid";

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
  
          //To process options        
          processOptions(userServer, mparameters, sqlListofMachines); 
         
          ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());
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
  ~ListMachinesServer()
  {
  }

  private:

  std::string mcommandName;
};

#endif
