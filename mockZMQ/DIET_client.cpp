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
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "utilVishnu.hpp"
#include "zhelpers.hpp"
#include "Server.hpp"
#include "SystemException.hpp"
#include "UserException.hpp"


// private declarations
int
diet_call_gen(diet_profile_t* prof, const std::string& uri);

typedef std::multimap<std::string, std::string> ConfigMap;
static ConfigMap theConfig;
const std::string NAMER_KEY = "namer";
const std::string TIMEOUT_KEY = "retryTimeout";

typedef std::map<std::string, std::string> ServiceMap;
static ServiceMap sMap = boost::assign::map_list_of
  /* UMS services */
  ("sessionConnect", "UMS")
  ("sessionReconnect", "UMS")
  ("sessionClose", "UMS")
  ("userCreate", "UMS")
  ("userUpdate", "UMS")
  ("userDelete", "UMS")
  ("userPasswordChange", "UMS")
  ("userPasswordReset", "UMS")
  ("machineCreate", "UMS")
  ("machineUpdate", "UMS")
  ("machineDelete", "UMS")
  ("localAccountCreate", "UMS")
  ("localAccountUpdate", "UMS")
  ("localAccountDelete", "UMS")
  ("configurationSave", "UMS")
  ("configurationRestore", "UMS")
  ("optionValueSet", "UMS")
  ("optionValueSetDefault", "UMS")
  ("sessionList", "UMS")
  ("localAccountList", "UMS")
  ("machineList", "UMS")
  ("commandList", "UMS")
  ("optionValueList", "UMS")
  ("userList", "UMS")
  ("restore", "UMS")
  ("authSystemCreate", "UMS")
  ("authSystemUpdate", "UMS")
  ("authSystemDelete", "UMS")
  ("authSystemList", "UMS")
  ("authAccountCreate", "UMS")
  ("authAccountUpdate", "UMS")
  ("authAccountDelete", "UMS")
  ("authAccountList", "UMS")
/* TMS services */
  ("jobSubmit", "TMS")
  ("jobCancel", "TMS")
  ("jobInfo", "TMS")
  ("getListOfJobs", "TMS")
  ("getJobsProgression", "TMS")
  ("getListOfQueues", "TMS")
  ("jobOutputGetResult", "TMS")
  ("jobOutputGetCompletedJobs", "TMS")
  ("addWork", "TMS")
/* IMS services */
  ("int_exportCommands", "IMS")
  ("int_getMetricCurentValue", "IMS")
  ("int_getMetricHistory", "IMS")
  ("int_getProcesses", "IMS")
  ("int_setSystemInfo", "IMS")
  ("int_setSystemThreshold", "IMS")
  ("int_getSystemThreshold", "IMS")
  ("int_defineUserIdentifier", "IMS")
  ("int_defineJobIdentifier", "IMS")
  ("int_defineTransferIdentifier", "IMS")
  ("int_defineMachineIdentifier", "IMS")
  ("int_loadShed", "IMS")
  ("int_setUpdateFrequency", "IMS")
  ("int_getUpdateFrequency", "IMS")
  ("int_restart", "IMS")
  ("int_stop", "IMS")
  ("int_getSystemInfo", "IMS")
  ("int_defineAuthIdentifier", "IMS")
  ("int_defineWorkIdentifier", "IMS")
/* FMS services */
  ("FileCopyAsync", "FMS")
  ("FileMoveAsync", "FMS")
  ("FileMove", "FMS")
  ("FileCopy", "FMS")
  ("FileGetInfos", "FMS")
  ("FileChangeGroup", "FMS")
  ("FileChangeMode", "FMS")
  ("FileHead", "FMS")
  ("FileContent", "FMS")
  ("FileCreate", "FMS")
  ("DirCreate", "FMS")
  ("FileRemove", "FMS")
  ("DirRemove", "FMS")
  ("FileTail", "FMS")
  ("DirList", "FMS")
  ("RemoteFileCopyAsync", "FMS")
  ("RemoteFileMoveAsync", "FMS")
  ("RemoteFileCopy", "FMS")
  ("RemoteFileMove", "FMS")
  ("FileTransfersList", "FMS")
  ;

std::string
get_module(const std::string& service) {

  std::size_t pos = service.find("@");
  if (std::string::npos == pos) {
    return sMap[service];
  } else {
    return sMap[service.substr(0, pos)];
  }

  return sMap[service];
}


void
fill(ConfigMap& cfg, const std::string& mfile) {

  std::ifstream tfile(mfile.c_str());

  if (tfile) {
    std::string line;
    std::vector<std::string> buff;

    while(std::getline(tfile, line)) {
      if(boost::algorithm::starts_with("#", line)) {
        continue;
      }

      boost::algorithm::trim(line);
      boost::algorithm::split(buff, line, boost::is_any_of("="));

      if (buff.size() != 2) {
        // we skip faulty entries
        std::cerr <<boost::format("E: invalid line in config file %1%: %2%\n")	% mfile % line;
        continue;
      }

      cfg.insert(std::make_pair(boost::algorithm::trim_copy(buff[0]), boost::algorithm::trim_copy(buff[1])));
    }
  } else {
    throw SystemException(ERRCODE_SYSTEM, "E: failed to open file for initializing the client\n");
  }
}


int
getTimeout() {

  int timeout = DEFAULT_TIMEOUT;

  ConfigMap::iterator iter = theConfig.find(TIMEOUT_KEY);
  if(iter != theConfig.end()) {
    timeout = vishnu::convertToInt(iter->second);
    if (timeout <= 0) {
      timeout = DEFAULT_TIMEOUT;
    }
  }
  return timeout;
}


diet_profile_t*
diet_profile_alloc(const char* name, int IN, int INOUT, int OUT) {
// TODO : Do not handle -1 for input (no input param)
  diet_profile_t* res = (diet_profile_t*) malloc(sizeof(diet_profile_t)*1);
  res->IN = IN;
  res->INOUT = INOUT;
  res->OUT = OUT;
  res->param = (char **)malloc(sizeof (char *)*(IN+INOUT+OUT+1));
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
  } else {
    ((diet_profile_t*)(arg->prof))->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen("")+1));
    memcpy(((diet_profile_t*)(arg->prof))->param[arg->pos], "", strlen(""));
    (((diet_profile_t*)(arg->prof))->param[arg->pos])[strlen("")] = '\0';
  }
  return 0;
}

void
sendProfile(diet_profile_t* prof, const std::string& uri) {
  zmq::context_t ctx(1);

  LazyPirateClient lpc(ctx, uri, getTimeout());

  std::string resultSerialized = my_serialize(prof);
  if (!lpc.send(resultSerialized)) {
    std::cerr << "E: request failed, exiting ...\n";
    throw SystemException(ERRCODE_SYSTEM, "Unable to contact the service");
  }

  // Receive response
  std::string response = lpc.recv();

  if (boost::starts_with(response, "error")) {
    throw SystemException(ERRCODE_SYSTEM, response);
  }
  //Update of profile
  boost::shared_ptr<diet_profile_t> tmp(my_deserialize(response.c_str()));
  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  for(int i = 0; i <= prof->OUT; ++i){
    prof->param[i] = strdup(tmp->param[i]);
  }
}

int
diet_call(diet_profile_t* prof) {
  std::vector<boost::shared_ptr<Server> > serv;
  std::string uri;

  // get the service and the related module
  std::string service(prof->name);

  //FIXME: to be removed
//  if(service.compare("jobSubmit@"+AUTOMATIC_SUBMIT_JOB_KEYWORD)==0) {
//	  if(selectMachine(prof) != 0) {
//	      throw SystemException(ERRCODE_SYSTEM, "Unable to select a machine for submiting the job");
//	  }
//	  return 0;
//  }

  std::string module = get_module(service);

  // check if the module has been declared in configuration.
  // otherwise, ask the naming service
  ConfigMap::iterator it = theConfig.find(module);
  if (it != theConfig.end()) {
    uri = it->second;
    return diet_call_gen(prof, uri);
  } else {
    it = theConfig.find(NAMER_KEY);
    if (theConfig.end() != it) {
      uri = it->second;
      sendProfile(prof, uri);
    } else {
      // basically you're screwed here
      throw SystemException(ERRCODE_SYSTEM,
                            boost::str(
                              boost::format("No corresponding %1% server found")
                              % service));
    }
  }

  //return diet_call_gen(prof, uri);

  return 0;
}

int
diet_call_gen(diet_profile_t* prof, const std::string& uri) {
  zmq::context_t ctx(5);
  LazyPirateClient lpc(ctx, uri, getTimeout());

  std::string s1 = my_serialize(prof);
  if (!lpc.send(s1)) {
    std::cerr << "E: request failed, exiting ...\n";
    return -1;
  }

  std::string response = lpc.recv();
  std::cout << boost::format("I: Recv> %1%...\n")%response;

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
  return 0;
}

int
diet_profile_free(diet_profile_t* prof){
  return 0;
}

diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos) {
  if (pos>prof->OUT){
    throw SystemException(ERRCODE_SYSTEM, "Invalid index, unallocated parameter");
  }
  diet_arg_t* res = (diet_arg_t*) malloc(sizeof(diet_arg_t)*1);
  res->prof = prof;
  res->pos = pos;
  return res;
}


std::string
my_serialize(diet_profile_t* prof) {
  if (prof==NULL){
    throw SystemException(ERRCODE_SYSTEM, "Cannot serialize a null pointer profile");
  }

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

  if (prof.compare("")==0){
    throw SystemException(ERRCODE_SYSTEM, "Cannot deserialize an empty string ");
  }

  boost::algorithm::split_regex(vecString, prof, boost::regex("\\${3}"));

  if (!vecString.empty() && vecString.at(0).compare("") != 0) {
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
  if (cfg==NULL){
    throw SystemException(ERRCODE_SYSTEM, "Invalid NULL initialization file");
  }
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

