//
// DIET_client.c
// Implémentation du mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB
//

#include "DIET_client.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include "utilVishnu.hpp"
#include "zhelpers.hpp"
#include "Server.hpp"
#include "SystemException.hpp"

static std::map<std::string, std::pair<std::string, std::string> > theConfig;

void
fill(std::map<std::string, std::pair<std::string, std::string> > &cfg, std::string mfile){
  std::ifstream tfile(mfile.c_str());

  if (tfile){
    std::string line;
    while(std::getline(tfile, line)){
      std::istringstream iss(line);
      std::string name;
      std::string addr;
      std::string port;
      iss >> name;
      iss >> addr;
      iss >> port;

      std::pair<std::string, std::string> mpair = std::pair<std::string, std::string>(addr, port);

      cfg.insert(std::pair<std::string, std::pair<std::string,std::string> > (name, mpair));

    }
  } else {
    std::cout << "failed to open file " << tfile << " for initialisation of client " << std::endl;
  }
}

diet_profile_t*
diet_profile_alloc(const char* name, int IN, int INOUT, int OUT) {
  diet_profile_t* res = (diet_profile_t*) malloc(sizeof(diet_profile_t)*1);
  res->IN = IN;
  res->INOUT = INOUT;
  res->OUT = OUT;
  res->param = (char **)malloc(sizeof (char *)*(IN+INOUT+OUT));
  memset(res->param,0,(IN+INOUT+OUT));
  res->name = (char *)malloc (sizeof(char) * (strlen(name)+1));
  memcpy(res->name, name, strlen(name));
  res->name[strlen(name)]='\0';
  return res;
}

int
diet_string_set(diet_arg_t* arg, char* value, int pers){
  if (value) {
    ((diet_profile_t*)(arg->prof))->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen(value)+1));
    memcpy(((diet_profile_t*)(arg->prof))->param[arg->pos], value, strlen(value));
    (((diet_profile_t*)(arg->prof))->param[arg->pos])[strlen(value)] = '\0';
//    std::cout << " Setting val to : " << arg->prof->param[arg->pos] << " for pos " << arg->pos << std::endl;
//    std::cout << " Setting val orig to : " << value << " for pos " << arg->pos << std::endl;
  } else {
    ((diet_profile_t*)(arg->prof))->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen("")+1));
    memcpy(((diet_profile_t*)(arg->prof))->param[arg->pos], "", strlen(""));
    (((diet_profile_t*)(arg->prof))->param[arg->pos])[strlen("")] = '\0';
  }
  return 0;
}

bool
isUMS(std::string test){
  return (
    test.compare("sessionConnect") == 0 ||
    test.compare("sessionReconnect") == 0 ||
    test.compare("sessionClose") == 0 ||
    test.compare("userCreate") == 0 ||
    test.compare("userUpdate") == 0 ||
    test.compare("userDelete") == 0 ||
    test.compare("userPasswordChange") == 0 ||
    test.compare("userPasswordReset") == 0 ||
    test.compare("machineCreate") == 0 ||
    test.compare("machineUpdate") == 0 ||
    test.compare("machineDelete") == 0 ||
    test.compare("localAccountCreate") == 0 ||
    test.compare("localAccountUpdate") == 0 ||
    test.compare("localAccountDelete") == 0 ||
    test.compare("configurationSave") == 0 ||
    test.compare("configurationRestore") == 0 ||
    test.compare("optionValueSet") == 0 ||
    test.compare("optionValueSetDefault") == 0 ||
    test.compare("sessionList") == 0 ||
    test.compare("localAccountList") == 0 ||
    test.compare("machineList") == 0 ||
    test.compare("commandList") == 0 ||
    test.compare("optionValueList") == 0 ||
    test.compare("userList") == 0 ||
    test.compare("restore") == 0 ||
    test.compare("authSystemCreate") == 0 ||
    test.compare("authSystemUpdate") == 0 ||
    test.compare("authSystemDelete") == 0 ||
    test.compare("authSystemList") == 0 ||
    test.compare("authAccountCreate") == 0 ||
    test.compare("authAccountUpdate") == 0 ||
    test.compare("authAccountDelete") == 0 ||
    test.compare("authAccountList") == 0);
}

bool
isTMS(std::string test) {
  /* TMS services are often postfixed by _<machineID>
     so we just check that the basename is present */
  return (
    test.find("jobSubmit") == 0 ||
    test.find("jobCancel") == 0 ||
    test.find("jobInfo") == 0 ||
    test.find("getListOfJobs") == 0 ||
    test.find("getJobsProgression") == 0 ||
    test.find("getListOfQueues") == 0 ||
    test.find("jobOutputGetResult") == 0 ||
    test.find("jobOutputGetCompletedJobs") == 0);
}


bool
isIMS(std::string test){
  std::cout << "test is : " << test << std::endl;
  std::cout << "bool : " << test.find("int_getMetricCurentValue") << std::endl;

  return (
    test.compare("int_exportCommands") == 0 ||
    test.find("int_getMetricCurentValue") == 0 ||
    test.compare("int_getMetricHistory") == 0 ||
    test.compare("int_getProcesses") == 0 ||
    test.compare("int_setSystemInfo") == 0 ||
    test.compare("int_setSystemThreshold") == 0 ||
    test.compare("int_getSystemThreshold") == 0 ||
    test.compare("int_defineUserIdentifier") == 0 ||
    test.compare("int_defineJobIdentifier") == 0 ||
    test.compare("int_defineTransferIdentifier") == 0 ||
    test.compare("int_defineMachineIdentifier") == 0 ||
    test.compare("int_loadShed") == 0 ||
    test.compare("int_setUpdateFrequency") == 0 ||
    test.compare("int_getUpdateFrequency") == 0 ||
    test.compare("int_restart") == 0 ||
    test.compare("int_stop") == 0 ||
    test.compare("int_getSystemInfo") == 0 ||
    test.compare("int_defineAuthIdentifier") == 0 ||
    test.compare("int_defineWorkIdentifier") == 0);
}

void
getServerAddresses(std::vector<boost::shared_ptr<Server> > &serv, std::string servname, int port, std::string addre){
  zmq::context_t ctx(1);
  boost::format addr("tcp://%1%:%2%");
  std::string uri = boost::str(addr % addre % port);
  LazyPirateClient lpc(ctx, uri);

  if (!lpc.send(servname)) {
    std::cerr << "E: request failed, exiting ...\n";
    exit(-1);
  }

  std::string response = lpc.recv();
  std::cout << "response received: ->" << response << "<- ," << response.size() <<  std::endl;
  if (response.size() <= 1) {
      throw SystemException(ERRCODE_SYSTEM, "No corresponding server found");
  }
  int precDol = response.find("$");
  std::string server;
  int tmp;
  while (precDol != std::string::npos) {
    tmp = response.find("$", precDol+1);
    std::cout << "token1: " << tmp << std::endl;
    if(tmp != std::string::npos){
      server = response.substr(precDol+1, tmp-precDol);
      std::cout << "server: " << server << std::endl;
    } else {
      server = response.substr(precDol+1, std::string::npos);
      std::cout << "server: " << server << std::endl;
    }
    precDol = tmp;

    std::string nameServ;
    std::string addr;
    int port;
    int prec;
    std::vector< std::string> vec;

    tmp = server.find("#", 0);
    prec = tmp;
    nameServ = server.substr(0, tmp);
    std::cout << "token2: " << nameServ << std::endl;
    std::cout << "cpt: " << prec << std::endl;
    tmp = server.find("#", prec+1);
    std::cout << "tmp found: " << tmp << std::endl;
    addr = server.substr(prec+1, tmp-prec-1);
    std::cout << "token3: " << addr << std::endl;
    port = vishnu::convertToInt(server.substr(tmp+1, std::string::npos));
    std::cout << "token4: " << port << std::endl;

    boost::shared_ptr<Server> s =boost::shared_ptr<Server>(new Server(nameServ, vec, addr, port));
    serv.push_back(s);
  }
}

boost::shared_ptr<Server>
electServer(std::vector<boost::shared_ptr<Server> > serv){
  if (serv.size() <= 0){
    throw SystemException(ERRCODE_SYSTEM, "No corresponding server found");
  }
  return serv.at(0);
}

int
diet_call(diet_profile_t* prof){
  int port;
  std::string addr;
  std::vector<boost::shared_ptr<Server> > serv;
  if (isUMS(std::string(prof->name))) {  // If UMS,
    if (theConfig.find("UMS") != theConfig.end()){ // search for it in config
      port = vishnu::convertToInt((theConfig.find("UMS")->second).second);
      addr = (theConfig.find("UMS")->second).first;
      diet_call_gen(prof, port, addr);
      } else if (theConfig.find("namer")!= theConfig.end()){ // Then ask name server
      port = vishnu::convertToInt((theConfig.find("namer")->second).second);
      addr = (theConfig.find("namer")->second).first;
      getServerAddresses(serv, prof->name, port, addr);
      boost::shared_ptr<Server> elected = electServer(serv);
      diet_call_gen(prof, elected.get()->getPort(), elected.get()->getAddress());
    } else {
      throw SystemException(ERRCODE_SYSTEM, "No corresponding UMS server found");
    }
  } else if (isTMS(std::string(prof->name))) { // if tms
    if (theConfig.find("routage")!= theConfig.end()){ // look for the router
      port = vishnu::convertToInt((theConfig.find("routage")->second).second);
      addr = (theConfig.find("routage")->second).first;
      diet_call_gen(prof, port, addr);
    } else if (theConfig.find("namer")!= theConfig.end()){ // Then ask name server
      port = vishnu::convertToInt((theConfig.find("namer")->second).second);
      addr = (theConfig.find("namer")->second).first;
      getServerAddresses(serv, prof->name, port, addr);
      boost::shared_ptr<Server> elected = electServer(serv);
      diet_call_gen(prof, elected.get()->getPort(), elected.get()->getAddress());
    } else {
      throw SystemException(ERRCODE_SYSTEM, "No corresponding TMS server found");
    }
  } else if (isIMS(std::string(prof->name))) {
    if (theConfig.find("IMS")!= theConfig.end()){
      port = vishnu::convertToInt((theConfig.find("IMS")->second).second);
      addr = (theConfig.find("IMS")->second).first;
      diet_call_gen(prof, port, addr);
    } else if (theConfig.find("namer")!= theConfig.end()){ // Then ask name server
      port = vishnu::convertToInt((theConfig.find("namer")->second).second);
      addr = (theConfig.find("namer")->second).first;
      getServerAddresses(serv, prof->name, port, addr);
      boost::shared_ptr<Server> elected = electServer(serv);
      diet_call_gen(prof, elected.get()->getPort(), elected.get()->getAddress());
    } else {
      throw SystemException(ERRCODE_SYSTEM, "No corresponding IMS server found");
    }
  } else {
    if (theConfig.find("FMS")!= theConfig.end()){
      port = vishnu::convertToInt((theConfig.find("FMS")->second).second);
      addr = (theConfig.find("FMS")->second).first;
      diet_call_gen(prof, port, addr);
    } else if (theConfig.find("namer")!= theConfig.end()){ // Then ask name server
      port = vishnu::convertToInt((theConfig.find("namer")->second).second);
      addr = (theConfig.find("namer")->second).first;
      getServerAddresses(serv, prof->name, port, addr);
      boost::shared_ptr<Server> elected = electServer(serv);
      diet_call_gen(prof, elected.get()->getPort(), elected.get()->getAddress());
    } else {
      throw SystemException(ERRCODE_SYSTEM, "No corresponding FMS server found");
    }
  }

  return 0;
}

int
diet_call_gen(diet_profile_t* prof, int my_port, std::string addre) {
  zmq::context_t ctx(1);
  boost::format addr("tcp://%1%:%2%");
  std::string uri = boost::str(addr % addre % my_port);
  LazyPirateClient lpc(ctx, uri);

  std::string s1 = my_serialize(prof);

  if (!lpc.send(s1)) {
    std::cerr << "E: request failed, exiting ...\n";
    exit(-1);
  }

  std::string response = lpc.recv();

  boost::shared_ptr<diet_profile_t> tmp(my_deserialize(response.c_str()));

  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  for(int i = 0; i <= prof->OUT; ++i){
    prof->param[i] = strdup(tmp->param[i]);
  }

  return 0;
}


int
diet_string_get(diet_arg_t* arg, char** value, void* ptr) {
  *value = (char *)malloc((strlen(((diet_profile_t*)(arg->prof))->param[arg->pos])+1)*sizeof (char));
  memcpy(*value, ((diet_profile_t*)(arg->prof))->param[arg->pos], strlen(((diet_profile_t*)(arg->prof))->param[arg->pos]));
  (*value)[strlen(((diet_profile_t*)(arg->prof))->param[arg->pos])]='\0';
//  std::cout << "Value set :" << *value << std::endl;
  return 0;
}

int
diet_profile_free(diet_profile_t* prof){
  return 0;
}

diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos) {
  diet_arg_t* res = (diet_arg_t*) malloc(sizeof(diet_arg_t)*1);
  res->prof = prof;
  res->pos = pos;
  return res;
}


std::string
my_serialize(diet_profile_t* prof) {
  std::stringstream res;
  res << prof->name <<  "$$$"
      << prof->IN << "$$$"
      << prof->INOUT << "$$$"
      << prof->OUT << "$$$";
  for (int i = 0; i<(prof->OUT); ++i) {
    res << prof->param[i] << "$$$";
  }
  res << prof->param[(prof->OUT)];
  return res.str();
}

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof) {
  boost::shared_ptr<diet_profile_t> res;
  std::vector<int> vec;

  std::vector<std::string> vecString;
  boost::algorithm::split_regex(vecString, prof, boost::regex("\\${3}"));

  if (!vecString.empty()) {
    res.reset(new diet_profile_t);
    std::vector<std::string>::iterator it = vecString.begin();
    res->name = strdup((it++)->c_str());
    res->IN = boost::lexical_cast<int>(*(it++));
    res->INOUT = boost::lexical_cast<int>(*(it++));
    res->OUT = boost::lexical_cast<int>(*(it++));
    res->param = (char**)malloc(sizeof(char*) * vecString.size() - 4);
    for (int i = 0; it != vecString.end(); it++, i++) {
      res->param[i] = strdup(it->c_str());
    }
  }

  return res;
}

int
diet_initialize(const char* cfg, int argc, char** argv) {
  fill(theConfig, std::string(cfg));
  return 0;
}

int
diet_finalize() {
  return 0;
}

int
diet_container_set(diet_arg_t* arg, int flag) {
}
