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
  std::string uriSed;
  std::string mid;
  std::string uriDispatcher;
  std::string uriSupervisor;
  int timeout = 10;

  // Getting the machine id
  if (type == "umssed") { // if ums
    config.getConfigValue<std::string>(vishnu::MACHINEID, mid);
  }
  else {
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
  }
  config.getConfigValue<int>(vishnu::TIMEOUT, timeout);
  if (timeout <= 0) {
    timeout = 10;
  }

  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, uriDispatcher);
  if (type == "fmssed") {
    config.getRequiredConfigValue<std::string>(vishnu::FMS_URIADDR, uriSed);
  } else  if (type == "imssed") {
    config.getRequiredConfigValue<std::string>(vishnu::IMS_URIADDR, uriSed);
  } else  if (type == "tmssed") {
    config.getRequiredConfigValue<std::string>(vishnu::TMS_URIADDR, uriSed);
  } else { // presumably UMS
    config.getRequiredConfigValue<std::string>(vishnu::UMS_URIADDR, uriSed);
  }

  /* Validate the uri and registry the SeD if not yet the case */
  validateUri(uriDispatcher);

  // Check whether to use supervisord
  if (config.getConfigValue<std::string>(vishnu::URISUPERVISOR, uriSupervisor) &&
      vishnu::isNew(uriSupervisor, mid, type)) {
    try {
      DbFactory factory;
      Database* database = factory.getDatabaseInstance();

      std::string request = (boost::format("INSERT INTO process (dietname, launchscript,"
                                           "            machineid, pstatus, uptime, vishnuname)"
                                           " VALUES ('%1%','%2%','%3%',%4%,CURRENT_TIMESTAMP, '%5%')")
                             %uriSupervisor
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

  boost::shared_ptr<Server> srv = boost::shared_ptr<Server> (new Server(type, services, uriSed));
  std::string requestData = "1" + srv.get()->toString(); /* prefixed with 1 to say registering request */

  std::string response;
  bool useSsl = false;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) && useSsl) {

    std::string host;
    int port;
    host = getHostFromUri(uriDispatcher);
    if (host.empty()) {
      std::cerr << boost::format("[ERROR] *** Incorrect host address from the service uri (%1%) ***\n")%uriDispatcher;
      /* %RELAX<MISRA_18_0_3> Because this error occurs in the forked process,
       * abort allows to exit the whole program, intstead of having defunct process. */
      abort();
    }

    port = getPortFromUri(uriDispatcher);
    if (port <= 0 ) {
      std::cerr << boost::format("[ERROR] *** Incorrect port from the service uri (%1%) ***\n")%uriDispatcher;
      abort();
    }

    /* Get TLS trust store if set */
    std::string cafile;
    config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);

    /* Logging */
    std::cerr << boost::format("[INFO] Registration address: %1%:%2% with TLS\n"
                               "       > Certifcate trust store (CA): %3%\n") %host %port %cafile;


    /* Now create a TLS client and go ahead */
    TlsClient tlsClient(host, port, cafile);

    requestData.append("\n\n");      /* required for the internal protocol */
    if (tlsClient.send(requestData) == 0) {
      response = tlsClient.recv();
    } else {
      std::cerr << boost::format("[ERROR] Registration failed [%1%]\n")%tlsClient.getErrorMsg();
      return -1;
    }
  } else {
    zmq::context_t ctx(1);
    LazyPirateClient lpc(ctx, uriDispatcher, timeout);
    if (!lpc.send(requestData)) {
      std::cerr << "[WARNING] failed to register in the naming service\n";
      return -1; // Dont throw exception
    }
    response = lpc.recv();
  }

  return 0;
}

void
initSeD(const std::string& type,
        const ExecConfiguration& config,
        const std::string& uri,
        boost::shared_ptr<SeD> server) {

  const std::string IPC_URI = (boost::format("ipc:///tmp/vishnu-%1%.sock")%type).str();

  try {
    std::vector<std::string> services = server.get()->getServices();
    registerSeD(type, config, services);
  } catch (VishnuException& e) {
    std::cout << boost::format("[WARNING] Failed registering the service (%1%) ***\n")%e.what();
  }

  bool useSsl = false;
  if (! config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) ||
      ! useSsl) { /* TLS dont required */
    ZMQServerStart(server, uri, false, "");
  } else {
    pid_t pid = fork();
    if (pid > 0) {
      std::string cafile;
      config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
      ZMQServerStart(server, IPC_URI, useSsl, cafile);
    } else if (pid == 0) {

      /* Intializing the TLS listener if necessary */
      int sslPort = getPortFromUri(uri);
      if (sslPort <= 0 ) {
        std::cerr << boost::format("[ERROR] *** Incorrect uri set for the service (%1%) ***\n")%uri;
        abort();
      }
      std::string rsaPrivkey;
      std::string sslCertificate;
      config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivkey);
      config.getRequiredConfigValue<std::string>(vishnu::SERVER_SSL_CERTICATE, sslCertificate);
      TlsServer tlsHandler(rsaPrivkey, sslCertificate, sslPort, IPC_URI);
      try {
        tlsHandler.run();
      } catch(VishnuException& ex) {
        std::cerr << boost::format("[ERROR] *** %1% ***\n")%ex.what();
        abort();
      } catch(...) {
        std::cerr << boost::format("[ERROR] *** %1% ***\n")%tlsHandler.getErrorMsg();
        abort();
      }
    } else {
      std::cerr << "[ERROR] *** Problem initializing the service ***\n";
      abort();
    }
  }
  unregisterSeD(type, config);
}

