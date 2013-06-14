//
// DIET_client.c
// Implémentation du mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB
//

#include "DIET_client.h"

#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>                    // for copy, transform
#include <stdexcept>                    // for out_of_range
#include <utility>                      // for pair
#include <boost/algorithm/string/predicate.hpp>  // for starts_with
#include <boost/algorithm/string/regex.hpp>  // for split_regex
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <zmq.hpp>                      // for context_t

#include "constants.hpp"                // for ::DISP_URIADDR, etc
#include "zhelpers.hpp"
#include "SystemException.hpp"
#include "ExecConfiguration.hpp"
#include "TMSServices.hpp"
#include "UMSServices.hpp"
#include "IMSServices.hpp"
#include "FMSServices.hpp"
#include "utilVishnu.hpp"

// private declarations
static ExecConfiguration config;

typedef std::map<std::string, std::string> ServiceMap;
boost::shared_ptr<ServiceMap> sMap;

// defines minimal number of elements in a diet profile
static const int PROFILE_ELT_NB = 4;


static void
fill_sMap() {
  unsigned int nb;
  sMap = boost::shared_ptr<ServiceMap>(new ServiceMap);
  /* UMS services */
  for (nb = 0; nb < NB_SRV_UMS; nb++) {
    (*sMap)[SERVICES_UMS[nb]] = "UMS";
  }

  /* TMS services */
  for (nb = 0; nb < NB_SRV_TMS; nb++) {
    (*sMap)[SERVICES_TMS[nb]] = "TMS";
  }

  /* IMS services */
  for (nb = 0; nb < NB_SRV_IMS; nb++) {
    (*sMap)[SERVICES_IMS[nb]] = "IMS";
  }

  /* FMS services */
  for (nb = 0; nb < NB_SRV_FMS; nb++) {
    (*sMap)[SERVICES_FMS[nb]] = "FMS";
  }
}


std::string
get_module(const std::string& service) {
  std::size_t pos = service.find("@");
  ServiceMap::const_iterator it;
  fill_sMap();

  if (std::string::npos != pos) {
    it = sMap->find(service.substr(0, pos));
  } else {
    it = sMap->find(service);
  }

  if (it != sMap->end()) {
    return it->second;
  }

  // Service not found
  return "";
}


/**
 * @brief getTimeout
 * @return
 */
int getTimeout() {
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
  if (!uriv.empty()) {
    std::istringstream iss(uriv[0]);
    iss >> uri;
  }
  config.getConfigValues(vishnu::DISP_URIADDR, dispv);
  if (!dispv.empty()) {
    disp = dispv[0];
  }

  if (uri.empty() && disp.empty()) {
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
  if (uri == "") {
    uri = disp;
  }

  bool useSsl = false;
  std::string cafile;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) && useSsl)
  {
    config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
    return ssl_call_gen(prof, getHostFromUri(uri), getPortFromUri(uri), cafile);
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
    std::cerr << boost::format("[ERROR] %1%\n")%response;
    return 1;
  }

  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  prof->params = tmp->params;
  return 0;
}

int
ssl_call_gen(diet_profile_t* prof,
             const std::string& host,
             const int& port,
             const std::string& cafile) {

  TlsClient tlsClient(host, port, cafile);

  if (tlsClient.send( my_serialize(prof) )) {
    std::cerr << boost::format("[ERROR] %1%\n")%tlsClient.getErrorMsg();
    return -1;
  }

  std::string response = tlsClient.recv();
  try {
    boost::shared_ptr<diet_profile_t> tmp(my_deserialize(response));
    if (tmp) {
      prof->IN = tmp->IN;
      prof->OUT = tmp->OUT;
      prof->INOUT = tmp->INOUT;
      prof->params = tmp->params;
      return 0;
    } else {
      std::cerr << boost::format("[ERROR] %1%\n")%response;
    }
  } catch (const VishnuException& ex) {
    std::cerr << boost::format("[ERROR] %1%\n")%ex.what();
  }
  return -1;
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
  res << prof->name <<  VISHNU_COMM_SEPARATOR
      << prof->IN << VISHNU_COMM_SEPARATOR
      << prof->INOUT << VISHNU_COMM_SEPARATOR
      << prof->OUT << VISHNU_COMM_SEPARATOR;

  for (int i = 0; i<(prof->OUT); ++i) {
    res << prof->params[i] << VISHNU_COMM_SEPARATOR;
  }
  res << prof->params[(prof->OUT)] << VISHNU_COMM_SEPARATOR;

  /*Crypt message before returning */

  return res.str();
}

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof) {
  boost::shared_ptr<diet_profile_t> res;

  std::vector<std::string> vecString;

  if (prof.empty()){
    throw SystemException(ERRCODE_SYSTEM, "Cannot deserialize an empty string ");
  }

  boost::algorithm::split_regex(vecString, prof, boost::regex(VISHNU_COMM_REGEX));

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

    if (res->params.size() <= res->OUT) {
      throw SystemException(ERRCODE_INVDATA, "Incoherent profile, wrong number of parameters");
    }
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


/**
 * @brief Get port number from a given uri
 * @param uri : the uri address
 * @return the port number
 */
int
getPortFromUri(const std::string& uri) {

  size_t pos = uri.rfind(":");
  if (pos == std::string::npos) {
    return -1;
  }
  return vishnu::convertToInt(uri.substr(pos+1, std::string::npos));
}

/**
 * @brief getHostFromUrl
 * @param uri
 * @return
 */
std::string
getHostFromUri(const std::string& uri) {

  size_t pos1 = uri.find("://");
  size_t pos2 = uri.rfind(":");

  if (pos1 == std::string::npos || pos2 == std::string::npos) {
    return std::string();
  }

  size_t pos = pos1+3;
  size_t len = pos2 - pos;
  return uri.substr(pos, len);
}
