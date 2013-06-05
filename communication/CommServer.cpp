#include "CommServer.hpp"

#include <boost/shared_ptr.hpp>

#include "Database.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "zmq.hpp"
#include "DIET_client.h"
#include "Server.hpp"
#include "zhelpers.hpp"
#include "SeD.hpp"

// TODO IMPLEMENT ME
int
unregisterSeD(const std::string& type, const ExecConfiguration& config) {
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
registerSeD(const std::string& type,
            const ExecConfiguration& config,
            std::vector<std::string>& services) {
  std::string uri;
  std::string mid;
  std::string uridispatcher;
  std::string urlsup;
  int timeout = 10;

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

  /* Validate the uri and registry the SeD if not yet the case */
  validateUri(uridispatcher);
  if (vishnu::isNew(urlsup, mid, type)) {
    try {
      DbFactory factory;
      Database* database = factory.getDatabaseInstance();

      std::string request = (boost::format("INSERT INTO process (dietname, launchscript,"
                                           "            machineid, pstatus, uptime, vishnuname)"
                                           " VALUES ('%1%','%2%','%3%',%4%,CURRENT_TIMESTAMP, '%5%')")
                             %urlsup
                             %config.scriptToString()
                             %mid
                             %vishnu::convertToString(vishnu::PRUNNING)
                             %type
                             ).str();

      database->process(request.c_str());
    } catch (...) {
      if (type == "umssed") { throw; }
    }
  }

  zmq::context_t ctx(1);
  LazyPirateClient lpc(ctx, uridispatcher, timeout);

  boost::shared_ptr<Server> srv = boost::shared_ptr<Server> (new Server(type, services, uri));
  std::string req = "1" + srv.get()->toString(); /* prefixed with 1 to say registering request */
  std::cerr << boost::format("[INFO] sendind-> %1%\n")%req;
  if (!lpc.send(req)) {
    std::cerr << "[WARNING] failed to register in the naming service\n";
    return -1; // Dont throw exception
  }
  std::string response = lpc.recv();

  return 0;
}

void
initSeD(const std::string& type,
        const ExecConfiguration& config,
        const std::string& uri,
        boost::shared_ptr<SeD> server) {

  const std::string IPC_URI = (boost::format("ipc:///tmp/vishnu-%1%.sock")%type).str();
  std::string rsaPrivkey;
  std::string sslCertificate;
  bool useSsl = false;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) &&  useSsl) {
    config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivkey);
    config.getRequiredConfigValue<std::string>(vishnu::SERVER_SSL_CERTICATE, sslCertificate);
  }
  pid_t pid = fork();
  if (pid > 0) {
    try {
      std::vector<std::string> services = server.get()->getServices();
      registerSeD(type, config, services);
    } catch (VishnuException& e) {
      std::cout << "Failed to register the SeD. " << e.what()  << std::endl;
    }
    ZMQServerStart(server, IPC_URI);
    unregisterSeD(type, config);
  } else if (pid == 0) {
    /* Intializing the SSL service */
    int sslPort = getPortFromUri(uri);
    if (sslPort <= 0 ) {
      sslPort = DEFAULT_SSL_PORT;
    }
    TlsServer tlsHandler(rsaPrivkey, sslCertificate, sslPort, IPC_URI);
    try {
      tlsHandler.run();
    } catch(VishnuException& ex) {
      std::cerr << boost::format("[ERROR] %1%")%ex.what();
      abort();
    } catch(...) {
      std::cerr << "[ERROR] unknown error when starting TLS server\n";
      abort();
    }
  } else {
    std::cerr << "[ERROR] There was a problem to initialize the SeD";
    exit(1);
  }
}

