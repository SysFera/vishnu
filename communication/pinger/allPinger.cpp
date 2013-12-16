#include "DIET_client.h"
#include "ExecConfiguration.hpp"
#include "Server.hpp"
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string/regex.hpp>  // for split_regex
#include <boost/algorithm/string/split.hpp>


#define PING_TIMEOUT 10


// Looking the service whose name starts by "service" and that is the longest
std::string
getLongestService(std::vector<std::string>& list, std::string service){
  std::string res = "";
  std::vector<std::string>::iterator it;
  for (it = list.begin() ; it != list.end() ; ++it){
    if (it->find(service) == 0 &&
	it->size() > res.size()){
      res = *it;
    }
  }
  return res;
}

int
main(int argc, char** argv){
  if (argc<2){
    std::cout << "usage : " << argv[0] << " <confFile> " << std::endl;
    exit(-1);
  }

  std::string requestData = "2"; /* prefixed with 2 to say get list or servers */
  std::string response;
  bool useSsl = false;
  ExecConfiguration config;
  std::string uriDispatcher;
  int timeout = PING_TIMEOUT;
  diet_initialize(argv[1], 0, NULL);
  config.initFromFile(argv[1]);
  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, uriDispatcher);
  config.getConfigValue<int>(vishnu::TIMEOUT, timeout);
  if (timeout <= 0) {
    timeout = PING_TIMEOUT;
  }
  zmq::context_t ctx(1);
  LazyPirateClient lpc(ctx, uriDispatcher, timeout);
  if (!lpc.send(requestData)) {
    std::cerr << "[ERROR] failed to list the servers to ping \n";
    return -1; // Dont throw exception
  }
  response = lpc.recv();
  // MAP ( URI , SERVERNAME )
  std::vector<boost::shared_ptr<Server> > servers;
  extractServersFromMessage(response, servers);

  // For each server ping it
  std::vector<boost::shared_ptr<Server> >::iterator it;
  for (it = servers.begin() ; it != servers.end() ; ++it){
    diet_profile_t* profile = NULL;
    std::string service = getLongestService(it->get()->getServices(), "heartbeat");
    profile = diet_profile_alloc(service,0,0,1);
    std::cout << "Trying to ping server " << it->get()->getName() << " located at " << it->get()->getURI() << "\n";
    // Pinging
    if (diet_call_gen(profile, it->get()->getURI())){
      std::cout << "[ERROR] Failed to ping he may be down or unreachable \n";
    } else {
      std::cout << "[SUCCESS] server answered \n";
    }
  }
}
