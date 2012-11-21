#include "Annuary.hpp"
#include <string>
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "utilVishnu.hpp"

Annuary::Annuary(){
  mservers = std::vector<boost::shared_ptr<Server> > ();
}

Annuary::Annuary(std::vector<boost::shared_ptr<Server> >& serv){
  mservers = serv;
}

Annuary::~Annuary(){
//  int i;
//  for (i=0;i<mservers.size();++i){
//    delete(mservers.at(i));
//  }
}


int
Annuary::add(std::string name, std::vector<std::string> services, std::string uri){
  boost::shared_ptr<Server> s;
  std::vector<boost::shared_ptr<Server> >* tmp = Annuary::get(services.at(0));
  std::vector<boost::shared_ptr<Server> >::iterator it;

// Do not add an already existing server
  for (it = tmp->begin(); it != tmp->end() ; ++it){
    if (name.compare(it->get()->getName()) == 0 &&
        uri.compare(it->get()->getURI()) == 0){
      return 0;
    }
  }
  s.reset (new Server(name, services, uri));
  mservers.push_back(s);
  return 0;
}


int
Annuary::remove(std::string name, std::string uri){
  int i;
  for (i=0;i<mservers.size();++i){
    if (name.compare(mservers.at(i)->getName()) == 0 &&
        uri.compare(mservers.at(i).get()->getURI()) == 0){
      mservers.erase(mservers.begin()+i);
      break;
    }
  }
  return 0;
}


std::vector<boost::shared_ptr<Server> >*
Annuary::get(std::string service){

  std::vector<boost::shared_ptr<Server> >* res= new std::vector<boost::shared_ptr<Server> >();
  if (service.compare("")==0){
    return &mservers;
  }
  for (int i=0; i<mservers.size(); i++){
    if (mservers.at(i)->hasService(service)){
      res->push_back(mservers.at(i));
    }
  }
  return res;
}


// Init the annuary from a file
void
Annuary::initFromFile(std::string infile){
  std::ifstream tfile(infile.c_str());

  if (tfile){
    std::string line;
    while(std::getline(tfile, line)){
      std::istringstream iss(line);
      std::string name;
      std::string uri;
      std::string mid;
      boost::shared_ptr<Server> server;
      std::vector<std::string> ports;
      iss >> name;
      iss >> uri;
      iss >> mid;
      std::vector<std::string> services;
      fillServices(services, name, mid);
      server = boost::shared_ptr<Server>(new Server(name, services, uri));
      mservers.push_back(server);
    }
  } else {
    std::cout << "failed to open file " << tfile << " for initialisation of annuary " << std::endl;
  }
}

void
Annuary::fillServices(std::vector< std::string> &services, std::string name, std::string mid){
  if (name.compare("UMS") == 0){
    services.push_back("sessionConnect") ;
    services.push_back("sessionReconnect") ;
    services.push_back("sessionClose") ;
    services.push_back("userCreate") ;
    services.push_back("userUpdate") ;
    services.push_back("userDelete") ;
    services.push_back("userPasswordChange") ;
    services.push_back("userPasswordReset") ;
    services.push_back("machineCreate") ;
    services.push_back("machineUpdate") ;
    services.push_back("machineDelete") ;
    services.push_back("localAccountCreate") ;
    services.push_back("localAccountUpdate") ;
    services.push_back("localAccountDelete") ;
    services.push_back("configurationSave") ;
    services.push_back("configurationRestore") ;
    services.push_back("optionValueSet") ;
    services.push_back("optionValueSetDefault") ;
    services.push_back("sessionList") ;
    services.push_back("localAccountList") ;
    services.push_back("machineList") ;
    services.push_back("commandList") ;
    services.push_back("optionValueList") ;
    services.push_back("userList") ;
    services.push_back("restore") ;
    services.push_back("authSystemCreate") ;
    services.push_back("authSystemUpdate") ;
    services.push_back("authSystemDelete") ;
    services.push_back("authSystemList") ;
    services.push_back("authAccountCreate") ;
    services.push_back("authAccountUpdate") ;
    services.push_back("authAccountDelete") ;
    services.push_back("authAccountList") ;
  } else if (name.compare("TMS") == 0) {
    services.push_back("jobSubmit@"+mid) ;
    services.push_back("jobCancel@"+mid) ;
    services.push_back("jobInfo@"+mid) ;
    services.push_back("getListOfJobs@"+mid) ;
    services.push_back("getJobsProgression@"+mid) ;
    services.push_back("getListOfQueues@"+mid) ;
    services.push_back("jobOutputGetResult@"+mid) ;
    services.push_back("jobOutputGetCompletedJobs@"+mid) ;
    services.push_back("getListOfJobs_all") ;
    services.push_back("jobSubmit_autom") ;
    services.push_back("addwork") ;
  } else if (name.compare("IMS") == 0) {
    services.push_back("int_exportCommands") ;
    services.push_back("int_getMetricCurentValue@"+mid) ;
    services.push_back("int_getMetricHistory") ;
    services.push_back("int_getProcesses") ;
    services.push_back("int_setSystemInfo") ;
    services.push_back("int_setSystemThreshold") ;
    services.push_back("int_getSystemThreshold") ;
    services.push_back("int_defineUserIdentifier") ;
    services.push_back("int_defineJobIdentifier") ;
    services.push_back("int_defineTransferIdentifier") ;
    services.push_back("int_defineMachineIdentifier") ;
    services.push_back("int_loadShed@"+mid) ;
    services.push_back("int_setUpdateFrequency") ;
    services.push_back("int_getUpdateFrequency") ;
    services.push_back("int_restart@"+mid) ;
    services.push_back("int_stop@"+mid) ;
    services.push_back("int_getSystemInfo") ;
    services.push_back("int_defineAuthIdentifier") ;
    services.push_back("int_defineWorkIdentifier") ;
  } else if (name.compare("FMS") == 0) {
    services.push_back("FileCopyAsync") ;
    services.push_back("FileMoveAsync") ;
    services.push_back("FileMove") ;
    services.push_back("FileCopy") ;
    services.push_back("FileGetInfos") ;
    services.push_back("FileChangeGroup") ;
    services.push_back("FileChangeMode") ;
    services.push_back("FileHead") ;
    services.push_back("FileContent") ;
    services.push_back("FileCreate") ;
    services.push_back("DirCreate") ;
    services.push_back("FileRemove") ;
    services.push_back("DirRemove") ;
    services.push_back("FileTail") ;
    services.push_back("DirList") ;
    services.push_back("RemoteFileCopyAsync") ;
    services.push_back("RemoteFileMoveAsync") ;
    services.push_back("RemoteFileCopy") ;
    services.push_back("RemoteFileMove") ;
    services.push_back("FileTransfersList") ;
    } else { // Routage
    services.push_back("routage");
  }
}

