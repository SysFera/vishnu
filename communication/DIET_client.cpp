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
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>  // for starts_with
#include <boost/algorithm/string/regex.hpp>  // for split_regex
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <zmq.hpp>                      // for context_t

#include "constants.hpp"                // for ::DISP_URIADDR, etc
#include "zhelpers.hpp"
#include "SystemException.hpp"
#include "ExecConfiguration.hpp"
#include "TMSServices.hpp"
#include "UMSServices.hpp"
#include "FMSServices.hpp"
#include "utilVishnu.hpp"

// private declarations
static ExecConfiguration config;

typedef std::map<std::string, std::string> ServiceMap;
boost::shared_ptr<ServiceMap> sMap;

static void
fill_sMap() {
  unsigned int nb;
  sMap = boost::make_shared<ServiceMap>();
  /* UMS services */
  for (nb = 0; nb < NB_SRV_UMS; nb++) {
    (*sMap)[SERVICES_UMS[nb]] = "UMS";
  }


  /* TMS services */
  for (nb = 0; nb < NB_SRV_TMS; nb++) {
    (*sMap)[SERVICES_TMS[nb]] = "TMS";
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
int
getTimeout() {
  int timeout(DEFAULT_TIMEOUT);
  config.getConfigValue(vishnu::TIMEOUT, timeout);
  if (timeout <= 0) {
    timeout = DEFAULT_TIMEOUT;
  }
  return timeout;
}


diet_profile_t*
diet_profile_alloc(const std::string &name, int nbparams) {
  diet_profile_t* res = new diet_profile_t;
  res->param_count = nbparams;
  (res->params).resize(nbparams, "");
  res->name = name;
  return res;
}

int
diet_string_set(diet_profile_t* prof, int pos,
                const std::string& value) {
  try {
    (prof->params).at(pos) = value;
  } catch (const std::out_of_range& err) {
    std::cout << boost::format("%1% %2% %3%\n") % prof->name
                 % value % pos;
    throw SystemException(ERRCODE_SYSTEM, "Invalid index, unallocated parameter");
  }
  return 0;
}


/**
 * @brief Reset the data and the number of parameters in the profile
 * @param prof The profile
 * @param nbparams The number of parameter
 */
void
diet_profile_reset(diet_profile_t* prof, int nbparams) {
  prof->params.clear();
  prof->param_count = nbparams;
  prof->params.resize(nbparams, "");
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
    param = vishnu::SED_URIADDR;
  } else if (module == "fms") {
    param = vishnu::SED_URIADDR;
  } else if (module == "tms") {
    param = vishnu::SED_URIADDR;
  } else {
    std::cerr << boost::format("No corresponding %1% server found\n") % service;
    return 1;
  }

  config.getConfigValues(param, uriv);
  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, service, boost::algorithm::is_any_of("@"));

  std::vector<boost::shared_ptr<Server> > allServers;
  extractMachineServersFromLine(uriv, allServers, "xmssed");

  config.getConfigValues(vishnu::DISP_URIADDR, dispv);
  if (!dispv.empty()) {
    disp = dispv[0];
  }

  if (allServers.size() == 0 && disp.empty()) {
    std::cerr << boost::format("No corresponding %1% server found\n") % service;
    return 1;
  }

  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = allServers.begin() ; it != allServers.end() ; ++it){
    try{
      int tmp = abstract_call_gen(prof, it->get()->getURI());
      if (tmp == 0)
        return 0;
    } catch (...){
    }
  }
  int tmp = 1;
  try{
    if ( !disp.empty() )
      tmp = abstract_call_gen(prof, disp);
  } catch (...){
  }

  if (tmp != 0)
    std::cerr << boost::format("No corresponding %1% server found") % service;


  return tmp;//abstract_call_gen(prof, uri);
}

int
diet_call_gen(diet_profile_t* prof, const std::string& uri, bool shortTimeout, int verbosity) {
  int timeout = shortTimeout?SHORT_TIMEOUT:getTimeout();
  zmq::context_t ctx(5);
  LazyPirateClient lpc(ctx, uri, timeout, verbosity);
  std::string s1 = my_serialize(prof);
  if (!lpc.send(s1)) {
    std::cerr << "E: request failed, exiting ...\n";
    return -1;
  }

  std::string response = lpc.recv();
  boost::shared_ptr<diet_profile_t> result(my_deserialize(response));
  if (! result) {
    std::cerr << boost::format("[ERROR] %1%\n")%response;
    return 1;
  }
  // To signal a communication problem (bad server receive request)
  // Otherwize client does not get any error message
  if (result->param_count == -1) {
    return 1;
  }

  prof->param_count = result->param_count;
  prof->params = result->params;
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
    boost::shared_ptr<diet_profile_t> resultProfile(my_deserialize(response));
    if (resultProfile) {
      prof->param_count = resultProfile->param_count;
      prof->params = resultProfile->params;
      return 0;
    }
    std::cerr << boost::format("[ERROR] %1%\n")%response;
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
  return JsonObject::serialize(prof);
}

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof) {
  return JsonObject::deserialize(prof);
}

int
diet_initialize(const char* cfg, int argc, char** argv) {
  if (!cfg) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid NULL initialization file");
  }
  config.initFromFile(cfg);
  return 0;
}

int
communicate_dispatcher(const std::string& requestData, std::string& response, bool shortTimeout, int verbosity){
  int timeout = shortTimeout?SHORT_TIMEOUT:getTimeout();
  std::string uriDispatcher;
  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, uriDispatcher);
  bool useSsl = false;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) && useSsl) {
    std::string req = requestData;
    std::string host;
    int port;
    host = vishnu::getHostFromUri(uriDispatcher);
    port = vishnu::getPortFromUri(uriDispatcher);
    std::string cafile;
    config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
    TlsClient tlsClient(host, port, cafile);
    req.append("\n\n");      /* required for the internal protocol */
    if (tlsClient.send(req) == 0) {
      response = tlsClient.recv();
    }
  } else {
    zmq::context_t ctx(1);
    LazyPirateClient lpc(ctx, uriDispatcher, timeout, verbosity);
    if (!lpc.send(requestData)) {
      return -1; // Dont throw exception
    }
    response = lpc.recv();
  }

  return 0;
}

void
extractServersFromMessage(std::string msg, std::vector<boost::shared_ptr<Server> >& res){
  using boost::algorithm::split_regex;
  std::vector<std::string> vecString;
  split_regex(vecString, msg, boost::regex(VISHNU_COMM_REGEX));
  std::vector<std::string>::iterator it;
  for (it = vecString.begin() ; it != vecString.end() ; ++it){
    std::string val(it->c_str()); // CREATE A STRING BECAUSE IT->EMPTY = FALSE WHEN IT EMPTY
    // URI is the key
    if ( !val.empty()){
      res.push_back(Server::fromString(val));
    }
  }
}

int
abstract_call_gen(diet_profile_t* prof, const std::string& uri, bool shortTimeout, int verbosity){
  bool useSsl = false;
  std::string cafile;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) && useSsl)
  {
    config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
    return ssl_call_gen(prof, vishnu::getHostFromUri(uri), vishnu::getPortFromUri(uri), cafile);
  }
  return diet_call_gen(prof, uri, shortTimeout, verbosity);
}


void
extractServersFromLine(const std::vector<std::string>& uriv, std::vector<boost::shared_ptr<Server> >& allServers, const std::string& module){
  using boost::algorithm::split_regex;
  std::vector<std::string> tokens;
  std::vector<std::string> tmp;
  tmp.push_back("heartbeat");
  std::vector<std::string>::iterator it;
  std::string uri;

  BOOST_FOREACH(const std::string& v, uriv) {
    boost::algorithm::split(tokens, v, boost::algorithm::is_any_of(";")); // In client config files, URI are separated by ;
    for (it = tokens.begin() ; it != tokens.end() ; ++it){
      uri = *it;
      allServers.push_back(boost::make_shared<Server>(module, tmp, uri));
    }
    tokens.clear();
  }
}

void
extractMachineServersFromLine(const std::vector<std::string>& uriv, std::vector<boost::shared_ptr<Server> >& allServers, const std::string& module){
  using boost::algorithm::split_regex;
  std::vector<std::string> tokens;
  std::vector<std::string> tokens2;
  std::vector<std::string> tmp;
  tmp.push_back("heartbeat");
  std::vector<std::string>::iterator it;
  std::string uri;

  BOOST_FOREACH(const std::string& v, uriv) {
    boost::algorithm::split(tokens, v, boost::algorithm::is_any_of(";")); // In client config files, URI are separated by ;
    BOOST_FOREACH(const std::string& w, tokens) {
      boost::algorithm::split(tokens2, w, boost::algorithm::is_space());
      int pos = 0;
      if (tokens2[0].empty() && tokens2.size()>1)
        pos = 1;
      uri = tokens2[pos];
      if ( tokens2.size()>pos+1 && !tokens2[pos+1].empty() ){
        tmp.push_back("heartbeatxmssed@"+tokens2[pos+1]);
      }
      allServers.push_back(boost::make_shared<Server>(module, tmp, uri));
      tokens2.clear();
    }
    tokens.clear();
  }
}


void
getServersListFromConfig(std::vector<boost::shared_ptr<Server> >& allServers){
  vishnu::param_type_t param;
  vishnu::param_type_t tms;
  std::vector<std::string> uriv;
  bool hastms(false);
  param = vishnu::SED_URIADDR;
  tms = vishnu::HAS_TMS;
  config.getConfigValues(param, uriv);
  config.getConfigValue<bool>(tms, hastms);
  if (uriv.size() > 0) {
      extractMachineServersFromLine(uriv, allServers, "xmssed");
  }
}


/**
 * @brief Raise exception when profile hold error status
 * @param profile The profile to analyse
 */
void
raiseExceptionOnErrorResult(diet_profile_t* profile) {
  if (profile && profile->param_count == 2) {
    std::string status;
    std::string msg;
    diet_string_get(profile, 0, status);
    if (status != "success") {
      diet_string_get(profile, 1, msg);
      diet_profile_free(profile);
      throw SystemException(ERRCODE_SYSTEM, msg);
    }
  } else {
    if (profile) {
      diet_profile_free(profile);
    }
    throw SystemException(ERRCODE_INVDATA, "Bad result profile");
  }
}


