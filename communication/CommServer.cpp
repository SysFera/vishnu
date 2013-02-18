#include "CommServer.hpp"
#include "Database.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "zmq.hpp"
#include "DIET_client.h"
#include "Server.hpp"
#include "zhelpers.hpp"

int
unregisterSeD(std::string type, ExecConfiguration config) {
  return 0;
}

void
validateUri(const std::string & uri) {
  size_t pos = uri.find("*");
  if (pos != std::string::npos) {
    throw UserException(ERRCODE_INVALID_PARAM,
                        std::string("W: character '*' is not permitted in the uri"));
  }
}

int
registerSeD(std::string type, ExecConfiguration config,
            std::string& cfg, std::vector<std::string>& services) {
  std::string uri;
  std::string mid;
  std::string uridispatcher;
  std::string urlsup;
  int timeout;

  // Getting the machine id
  config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
  config.getConfigValue<int>(vishnu::TIMEOUT, timeout);
  if (timeout <= 0) {
    timeout = 10;
  }

  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, uridispatcher);
  config.getRequiredConfigValue<std::string>(vishnu::URLSUPERVISOR, urlsup);
  if (type == "fmssed") {
    config.getRequiredConfigValue<std::string>(vishnu::FMS_URIADDR, uri);
  } else  if (type == "imssed") {
    config.getRequiredConfigValue<std::string>(vishnu::IMS_URIADDR, uri);
  } else  if (type == "tmssed") {
    config.getRequiredConfigValue<std::string>(vishnu::TMS_URIADDR, uri);
  } else { // presumably UMS
    config.getRequiredConfigValue<std::string>(vishnu::UMS_URIADDR, uri);
  }

  // Check that the uri does not contain *
  validateUri(uridispatcher);

  // Register in database
  if (vishnu::isNew(urlsup, mid, type)) {
    std::string request = "insert into process (dietname, launchscript, machineid, pstatus, uptime, vishnuname) values ('"+urlsup+"','"+config.scriptToString()+"','"+mid+"','"+vishnu::convertToString(vishnu::PRUNNING)+"',CURRENT_TIMESTAMP, '"+type+"')";
    try {
      DbFactory factory;
      Database* database = factory.getDatabaseInstance();
      database->process(request.c_str());
    } catch (SystemException& e) {
      if (type.compare("umssed")!=0){
        throw (e);
      }
    }
  }

  zmq::context_t ctx(1);
  LazyPirateClient lpc(ctx, uridispatcher, timeout);

  boost::shared_ptr<Server> s = boost::shared_ptr<Server> (new Server(type, services, uri));
  // prefix with 1 to say registering the sed
  std::string req = "1" + s.get()->toString();

  std::cout << "sending " << req << std::endl;
  if (!lpc.send(req)) {
    std::cerr << "W: failed to register in the naming service\n";
    return -1; //instead of exiting
  }
  std::string response = lpc.recv();
  std::cout << "response received: ->" << response << "<- ," << response.length() <<  "\n";

  return 0;
}
