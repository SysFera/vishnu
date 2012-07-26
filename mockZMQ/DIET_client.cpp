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

static std::map<std::string, std::vector<std::string> > theConfig;

void
fill(std::map<std::string, std::vector<std::string> > &cfg, std::string mfile){
  std::ifstream file(mfile.c_str());

  if (file){
    std::string line;
    while(std::getline(file, line)){
      std::istringstream iss(line);
      std::string server;
      std::string port;
      std::vector<std::string> ports;
      iss >> server;
      while(iss >> port){
        ports.push_back(port);
      }
      cfg.insert(std::pair<std::string, std::vector<std::string> >(server, ports));
    }
  } else {
    std::cout << "failed to open file " << mfile << std::endl;
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


int
diet_call(diet_profile_t* prof){
  int port;
  if (isUMS(std::string(prof->name))) {
    port = vishnu::convertToInt((theConfig.find("UMS")->second).at(0));
    diet_call_gen(prof, port);
  } else if (isTMS(std::string(prof->name))) {
    port = vishnu::convertToInt((theConfig.find("TMS")->second).at(0));
    diet_call_gen(prof, port);
  } else if (isIMS(std::string(prof->name))) {
    port = vishnu::convertToInt((theConfig.find("IMS")->second).at(0));
    diet_call_gen(prof, port);
  } else {
    port = vishnu::convertToInt((theConfig.find("FMS")->second).at(0));
    diet_call_gen(prof, port);
  }



//  try {

//  zmq::context_t ctx(1);
//  zmq::socket_t sock(ctx, ZMQ_REQ);
////  std::string address;
////  address = std::string(prof->address);
////  address += ":";
////  address += vishnu::convertToString<int>(prof->port);
//  sock.connect("tcp://localhost:5555");
////  std::cerr << "send: \"" << my_serialize(prof).c_str() << std::endl;
//  std::string s1 = my_serialize(prof);
//  zmq::message_t request(s1.length()+1);
//  memcpy((void*)request.data(), s1.c_str(), s1.length()+1);
//  sock.send(request);
//
//  zmq::message_t reply;
//  std::cout << "b4 recv " << std::endl;
//  sock.recv(&reply);
//  std::cout << "a4 recv " << std::endl;
////  std::cout << "Client receive : " << (char*)reply.data() << std::endl;
//
//  boost::shared_ptr<diet_profile_t>
//    tmp(my_deserialize(static_cast<char *>(reply.data())));
//  std::cout << "a5 recv " << std::endl;
////  *prof =
//  prof->IN = tmp->IN;
//  prof->OUT = tmp->OUT;
//  prof->INOUT = tmp->INOUT;
//  for(int i=0;i<=prof->OUT;++i){
//    prof->param[i] = strdup(tmp->param[i]);
//  }
//  std::cout << "error:  " << prof->param[prof->OUT] << std::endl;
//  if (strcmp(prof->param[prof->OUT], "")!=0){
//    std::cout << "a7 recv " << std::endl;
//    diet_call_fms(prof);
//  }
//
//  std::cout << "a6 recv " << std::endl;
//  } catch (...){
//    std::cout << "Failed -> in exception " << std::endl;
//    diet_call_fms(prof);
//  }

  return 0;
}

int
diet_call_gen(diet_profile_t* prof, int my_port){
  zmq::context_t ctx(1);
  zmq::socket_t sock(ctx, ZMQ_REQ);
//  std::string address;
//  address = std::string(prof->address);
//  address += ":";
  std::string toto  = vishnu::convertToString<int>(my_port);
  sock.connect(std::string("tcp://localhost:"+toto).c_str());
//  std::cerr << "send: \"" << my_serialize(prof).c_str() << std::endl;
  std::string s1 = my_serialize(prof);
  zmq::message_t request(s1.length()+1);
  memcpy((void*)request.data(), s1.c_str(), s1.length()+1);
  sock.send(request);

  zmq::message_t reply;
  sock.recv(&reply);
//  std::cout << "Client receive : " << (char*)reply.data() << std::endl;

  boost::shared_ptr<diet_profile_t>
    tmp(my_deserialize(static_cast<char *>(reply.data())));
//  *prof =
  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  for(int i=0;i<=prof->OUT;++i){
    prof->param[i] = strdup(tmp->param[i]);
  }

  return 0;
}


int
diet_string_get(diet_arg_t* arg, char** value, void* ptr){
//  std::cout << "Getting arg in pos :" << arg->pos << " of value: " << arg->prof->param[arg->pos] << std::endl;
  *value = (char *)malloc((strlen(((diet_profile_t*)(arg->prof))->param[arg->pos])+1)*sizeof (char));
  memcpy(*value, ((diet_profile_t*)(arg->prof))->param[arg->pos], strlen(((diet_profile_t*)(arg->prof))->param[arg->pos]));
  (*value)[strlen(((diet_profile_t*)(arg->prof))->param[arg->pos])]='\0';
//  std::cout << "Value set :" << *value << std::endl;
//  std::cout << " Getting val to : " << arg->prof->param[arg->pos] << "for pos " << arg->pos << std::endl;
  return 0;
}

int
diet_profile_free(diet_profile_t* prof){
  return 0;
}

diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos){
  diet_arg_t* res = (diet_arg_t*) malloc(sizeof(diet_arg_t)*1);
  res->prof = prof;
  res->pos = pos;
  return res;
}


std::string
my_serialize(diet_profile_t* prof){
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
my_deserialize(const std::string& prof){
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
diet_initialize(const char* cfg, int argc, char** argv){
  fill(theConfig, std::string(cfg));
  return 0;
}

int
diet_finalize(){
  return 0;
}

int
diet_container_set(diet_arg_t* arg, int flag){

}
