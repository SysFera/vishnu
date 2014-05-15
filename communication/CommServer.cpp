#include "CommServer.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <unistd.h> // for sleep

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
#include "Logger.hpp"

/**
 * \brief Function to unregister a server (from the annuary and database)
 * \param type the type of the server
 * \param config the configuration file
 * \return 0 currently
 */
// TODO IMPLEMENT ME
int
unregisterSeD(const std::string& type, const ExecConfiguration& config) {
  return 0;
}


int
registerSeD(const std::string& sedUri,
            const std::string& dispUri,
            const std::string& sedType,
            const ExecConfiguration& config,
            std::vector<std::string>& services) {

  std::string mid;
  std::string uriSupervisor;

  // Getting the machine id
  if (sedType == "umssed") { // if ums
    config.getConfigValue<std::string>(vishnu::MACHINEID, mid);
  } else {
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
  }

  int timeout  = vishnu::DEFAUT_TIMEOUT;
  config.getConfigValue<int>(vishnu::TIMEOUT, timeout);

  if (config.getConfigValue<std::string>(vishnu::URISUPERVISOR, uriSupervisor) &&
      vishnu::isNew(uriSupervisor, mid, sedType)) {
    try {
      DbFactory factory;
      Database* database = factory.getDatabaseInstance();

      std::string request = (boost::format("INSERT INTO process (dietname, launchscript,"
                                           "            machineid, pstatus, uptime, vishnuname)"
                                           " VALUES ('%1%','%2%','%3%',%4%,CURRENT_TIMESTAMP, '%5%')")
                             %uriSupervisor
                             %database->escapeData(config.scriptToString())
                             %mid
                             %vishnu::convertToString(vishnu::PRUNNING)
                             %sedType
                             ).str();

      database->process(request.c_str());
    } catch (...) {
      if (sedType == "umssed") { throw; }
    }
  }

  return 0;
}

void
keepRegistered(const std::string& sedType,
               const ExecConfiguration& config,
               const std::string& sedUri,
               boost::shared_ptr<SeD> server){
  std::vector<std::string> services = server.get()->getServices();
  int timeout  = vishnu::DEFAUT_TIMEOUT;
  config.getConfigValue<int>(vishnu::TIMEOUT, timeout);
  std::string dispUri;
  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, dispUri);
  boost::shared_ptr<Server> srv = boost::make_shared<Server>(sedType, services, sedUri);
  std::string requestData = "1" + srv.get()->toString(); /* prefixed with 1 to say registering request */

  std::string response;
  bool useSsl = false;
  bool connected(false);
  while (true){
    if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) && useSsl) {

      std::string host;
      int port;
      host = vishnu::getHostFromUri(dispUri);
      port = vishnu::getPortFromUri(dispUri);

      /* Get TLS trust store if set */
      std::string cafile;
      config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
      /* Logging */
      /* Now create a TLS client and go ahead */
      TlsClient tlsClient(host, port, cafile);
      requestData.append("\n\n");      /* required for the internal protocol */
      if (tlsClient.send(requestData) == 0) {
        response = tlsClient.recv();
        if (response == "OK\n") { // \n at the end because it's in response see sslhelpers.cpp, \n is added at end of message
          if (!connected) {
            LOG("[INFO] Registered in dispatcher", LogInfo);
          }
          connected = true;
        } else {
          connected = false;
          LOG("[WARN] Not registered in dispatcher", LogInfo);
        }
      }
    } else {
      zmq::context_t ctx(1);
      LazyPirateClient lpc(ctx, dispUri, timeout);
      lpc.send(requestData);
      response = lpc.recv();
      if (response == "OK") {
        if (!connected) {
          LOG("[INFO] Registered in dispatcher", LogInfo);
        }
        connected = true;
      } else {
        connected = false;
        LOG("[WARN] Not registered in dispatcher", LogInfo);
      }
    }
    sleep(timeout);
  }
}


void
initSeD(const std::string& sedType,
        const ExecConfiguration& config,
        const std::string& sedUri,
        boost::shared_ptr<SeD> server) {

  // Init logging
  std::clog.rdbuf(new Logger("vishnu", LOG_LOCAL0));

  std::string ipcUriBase;
  if (!config.getConfigValue<std::string>(vishnu::IPC_URI_BASE, ipcUriBase)) {
    ipcUriBase = "/tmp/vishnu-";
  }

  const std::string IPC_URI = (boost::format("ipc://%1%%2%.sock")
                               % ipcUriBase % sedType).str();

  std::string dispUri;
  config.getRequiredConfigValue<std::string>(vishnu::DISP_URISUBS, dispUri);

  int nbthreads;
  if (! config.getConfigValue<int>(vishnu::NBTHREADS, nbthreads)) {
    nbthreads = 1;
  }

  // Validate the URIs
  vishnu::validateUri(sedUri);
  vishnu::validateUri(dispUri);

  try {
    std::vector<std::string> services = server.get()->getServices();
    registerSeD(sedUri, dispUri, sedType, config, services);
  } catch (VishnuException& e) {
    LOG(boost::str(boost::format("[WARNING] Failed registering the service (%1%)")
                   %e.what()), LogWarning);
  }

  bool useSsl = false;
  if (! config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) ||
      ! useSsl) { /* TLS dont required */
    ZMQServerStart(server, sedUri, nbthreads, false, "");
  } else {
    pid_t pid = fork();

    if (pid < 0) {  // Fork failed
      LOG("[ERROR]Problem initializing the service", LogErr);
      exit(-1);

    } else if (pid > 0) {  // Parent process

      std::string cafile;
      config.getConfigValue<std::string>(vishnu::SSL_CA, cafile);
      ZMQServerStart(server, IPC_URI, nbthreads, useSsl, cafile);

    } else if (pid == 0) { // Child process

      /* Intializing the TLS listener if necessary */
      int sslPort = vishnu::getPortFromUri(sedUri);
      std::string rsaPrivkey;
      std::string sslCertificate;
      config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivkey);
      config.getRequiredConfigValue<std::string>(vishnu::SERVER_SSL_CERTICATE, sslCertificate);
      TlsServer tlsHandler(rsaPrivkey, sslCertificate, sslPort, IPC_URI);
      int retCode = 0;
      try {
        tlsHandler.run();
      } catch(VishnuException& ex) {
        LOG(boost::str(boost::format("[ERROR] %1%\n")%ex.what()), LogErr);
        retCode = -1;
      } catch(...) {
        LOG(boost::str(boost::format("[ERROR] %1%\n")
                       % tlsHandler.getErrorMsg()), LogErr);
        retCode = -1;
      }
      vishnu::exitProcessOnError(retCode);
    }
  }
  unregisterSeD(sedType, config);
}

