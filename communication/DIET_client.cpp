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
#include <iterator>
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
#include "ExecConfiguration.hpp"


// private declarations
int
diet_call_gen(diet_profile_t* prof, const std::string& uri);

ExecConfiguration config;

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
                         ("getListOfJobs_all", "TMS")
                         ("jobSubmit_autom", "TMS")
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
                         ("FileTransferStop", "FMS")
                         ;

// defines minimal number of elements in a diet profile
static const int PROFILE_ELT_NB = 4;


std::string
get_module(const std::string& service) {
  std::size_t pos = service.find("@");
  ServiceMap::const_iterator it;

  if (std::string::npos != pos) {
    it = sMap.find(service.substr(0, pos));
  } else {
    it = sMap.find(service);
  }

  if (it != sMap.end()) {
    return it->second;
  }

  // Service not found
  return "";
}


inline int getTimeout() {
  int timeout = -1;
  if (!config.getConfigValue(vishnu::TIMEOUT, timeout) || timeout <= 0) {
    timeout =  DEFAULT_TIMEOUT;
  }
  return timeout;
}


diet_profile_t*
diet_profile_alloc(const std::string &name, int IN, int INOUT, int OUT) {
  // TODO : Do not handle -1 for input (no input param)
  diet_profile_t* res = new diet_profile_t;
  res->IN = IN;
  res->INOUT = INOUT;
  res->OUT = OUT;
  (res->params).resize(OUT+1, "");
  res->name = name;
  return res;
}

int
diet_string_set(diet_profile_t* prof, int pos,
                const std::string& value) {
  try {
    (prof->params).at(pos) = value;
  } catch (const std::out_of_range& err) {
    throw
    SystemException(ERRCODE_SYSTEM, "Invalid index, unallocated parameter");
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
  boost::shared_ptr<diet_profile_t> tmp(my_deserialize(response));
  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  prof->params = tmp->params;
}

int
diet_call(diet_profile_t* prof) {
  std::string uri;
  std::vector<std::string> uriv;
  std::string disp;
  std::vector<std::string> dispv;

  // get the service and the related module
  std::string service(prof->name);
  std::string module = get_module(service);
  std::transform(module.begin(), module.end(), module.begin(), ::tolower);

  vishnu::param_type_t param;
  if (module == "ums") {
    param = vishnu::UMS_URIADDR;
  } else if (module == "fms") {
    param = vishnu::FMS_URIADDR;
  } else if (module == "ims") {
    param = vishnu::IMS_URIADDR;
  } else if (module == "tms") {
    param = vishnu::TMS_URIADDR;
  } else {
    // Currently do not throw anything as diet_call is meant to return an error and not throw an exception
    // No module or server found
    // throw SystemException(ERRCODE_SYSTEM,
    //                       boost::str(
    //                         boost::format("No corresponding %1% server found")
    //                         % service));
    std::cerr << boost::format("No corresponding %1% server found") % service;
    return 1;
  }

  config.getConfigValues(param, uriv);
  if (uriv.size()>0){
    std::istringstream iss(uriv[0]);
    iss >> uri;
  }
  config.getConfigValues(vishnu::DISP_URIADDR, dispv);
  if (dispv.size()>0){
    disp = dispv[0];
  }

  if (uri == "" &&
      disp == "") {
    // Currently do not throw anything as diet_call is meant to return an error and not throw an exception
    // No module or server found
    // throw SystemException(ERRCODE_SYSTEM,
    //                       boost::str(
    //                         boost::format("No corresponding %1% server found")
    //                         % service));
    std::cerr << boost::format("No corresponding %1% server found") % service;
    return 1;
  }
  // If no direct data but dispatcher found
  if (uri == ""){
    uri = disp;
  }
  return diet_call_gen(prof, uri);
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
  // std::cout << boost::format("I: Recv> %1%...\n")%response;

  boost::shared_ptr<diet_profile_t> tmp(my_deserialize(response));
  if (!tmp) {
    std::cerr << boost::format("E: %1%...\n")%response;
    return 1;
  }

  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  prof->params = tmp->params;
  return 0;
}


int
diet_string_get(diet_profile_t* prof, int pos, std::string& value) {
  value = (prof->params).at(pos);
  return 0;
}

int
diet_profile_free(diet_profile_t* prof) {
  delete prof;
  return 0;
}


std::string
my_serialize(diet_profile_t* prof) {
  if (!prof){
    throw SystemException(ERRCODE_SYSTEM, "Cannot serialize a null pointer profile");
  }

  std::stringstream res;
  res << prof->name <<  "$$$"
      << prof->IN << "$$$"
      << prof->INOUT << "$$$"
      << prof->OUT << "$$$";

  for (int i = 0; i<(prof->OUT); ++i) {
    res << prof->params[i] << "$$$";
  }
  res << prof->params[(prof->OUT)] << "$$$";
  return res.str();
}

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof) {
  boost::shared_ptr<diet_profile_t> res;

  std::vector<std::string> vecString;

  if (prof.empty()){
    throw SystemException(ERRCODE_SYSTEM, "Cannot deserialize an empty string ");
  }

  boost::algorithm::split_regex(vecString, prof, boost::regex("\\${3}"));

  // TODO: this is not "safe" as we can receive errors int "prof"
  // Currently, we check that the size of vecString is at least 4:
  // profile's name, IN, INOUT and OUT
  if (vecString.size() >= PROFILE_ELT_NB && (vecString.at(0) != "")) {
    res.reset(new diet_profile_t);
    std::vector<std::string>::iterator it = vecString.begin();
    res->name = *(it++);
    res->IN = boost::lexical_cast<int>(*(it++));
    res->INOUT = boost::lexical_cast<int>(*(it++));
    res->OUT = boost::lexical_cast<int>(*(it++));

    std::copy(it, vecString.end(), std::back_inserter(res->params));
  }

  return res;
}

int
diet_initialize(const char* cfg, int argc, char** argv) {
  if (!cfg){
    throw SystemException(ERRCODE_SYSTEM, "Invalid NULL initialization file");
  }
  config.initFromFile(cfg);
  return 0;
}
