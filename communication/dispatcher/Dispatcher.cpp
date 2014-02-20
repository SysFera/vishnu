#include "Dispatcher.hpp"
#include "Server.hpp"
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include "utilVishnu.hpp"
#include "DIET_client.h"
#include "VishnuException.hpp"
#include <signal.h>


Dispatcher::Dispatcher(const std::string &confFile)
  : uriAddr("tcp://127.0.0.1:5560"),
    uriSubs("tcp://127.0.0.1:5561"), confFil(confFile), nthread(5), timeout(10) {
  if (!confFile.empty()) {
    config.initFromFile(confFile);
  }
}


void
Dispatcher::printConfiguration() const{
  std::cerr << "====== Initial configuration =====\n"
            << "disp_uriAddr=" << uriAddr << "\n"
            << "disp_uriSubs=" << uriSubs << "\n"
            << "disp_timeout=" << timeout << "\n"
            << "disp_nbthread=" << nthread << "\n"
            << "==================================\n";
}


void
Dispatcher::readConfiguration() {
  config.getConfigValue<std::string>(vishnu::DISP_URIADDR, uriAddr);
  config.getConfigValue<std::string>(vishnu::DISP_URISUBS, uriSubs);
  config.getConfigValue<unsigned int>(vishnu::DISP_NBTHREAD, nthread);
  config.getConfigValue<unsigned int>(vishnu::TIMEOUT, timeout);
  printConfiguration();
}

void
Dispatcher::configureAnnuary() {
  // Prepare our context and socket
  ann = boost::make_shared<Annuary>();
  std::string mid;
  config.getConfigValue<std::string>(vishnu::MACHINEID, mid);

  // Get initial configuration
  std::vector<std::string> cfgInfo;
  if (config.getConfigValues(vishnu::FMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("fmssed", cfgInfo, mid);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::IMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("imssed", cfgInfo, mid);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::TMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("tmssed", cfgInfo, mid);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::UMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("umssed", cfgInfo, mid);
    cfgInfo.clear();
  }
  ann->print();
}

boost::shared_ptr<Annuary>
Dispatcher::getAnnuary(){
  return ann;
}


void
Dispatcher::bayWatch(boost::shared_ptr<Annuary> ann, int timeout, std::string& confFile){
  try {
    diet_initialize(confFile.c_str(), 0, NULL);
  } catch (VishnuException& e){
  }
  while (true){
    // get all servers
    std::vector<boost::shared_ptr<Server> > list = ann->get();
    std::vector<boost::shared_ptr<Server> >::iterator iter;
    std::string service = "heartbeat";
    // For each server
    for (iter = list.begin() ; iter != list.end() ; ++iter){
      diet_profile_t* profile = diet_profile_alloc(service, 0);
      // try to ping them
      if (abstract_call_gen(profile, iter->get()->getURI())){
        // If failed : remove the server
        ann->remove(iter->get()->getName(), iter->get()->getURI());
        std::cerr << "[INFO]: removed " << iter->get()->getName() << "@" << iter->get()->getURI() << " from the annuary\n";
      }
      diet_profile_free(profile);
    }
    // Sleep a bit
    sleep(timeout);
  }
}


void
Dispatcher::configureHandlers() {

  const std::string FRONTEND_IPC_URI = "ipc:///tmp/vishnu-disp.front.sock";
  const std::string BACKEND_IPC_URI = "ipc:///tmp/vishnu-disp.back.sock";

  bool useSsl = false;
  if (! config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) ||
      ! useSsl) { /* TLS dont required */
    clientHandler.reset(new Handler4Clients(uriAddr, ann, nthread, false, ""));
    serverHandler.reset(new Handler4Servers(uriSubs, ann, nthread, false, ""));
    boost::thread th1(boost::bind(&Handler4Clients::run, clientHandler.get()));
    boost::thread th2(boost::bind(&Handler4Servers::run, serverHandler.get()));
    boost::thread th3(boost::bind(&Dispatcher::bayWatch, ann, timeout, confFil));
    th1.join();
    th2.join();
    th3.join();
  } else { /* TLS required */
    std::string rsaPrivkey;
    std::string sslCertificate;
    std::string sslCa;
    config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivkey);
    config.getRequiredConfigValue<std::string>(vishnu::SERVER_SSL_CERTICATE, sslCertificate);
    config.getConfigValue<std::string>(vishnu::SSL_CA, sslCa);

    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "[ERROR] Problem initializing the service\n";
      vishnu::exitProcessOnError(-1);
    } else if (pid > 0) {
      clientHandler.reset(new Handler4Clients(FRONTEND_IPC_URI, ann, nthread, useSsl, sslCa));
      serverHandler.reset(new Handler4Servers(BACKEND_IPC_URI, ann, nthread, useSsl, sslCa));
      boost::thread th1(boost::bind(&Handler4Clients::run, clientHandler.get()));
      boost::thread th2(boost::bind(&Handler4Servers::run, serverHandler.get()));
      boost::thread th3(boost::bind(&Dispatcher::bayWatch, ann, timeout, confFil));
      th1.join();
      th2.join();
      th3.join();

      vishnu::exitProcessOnChildError(pid);

    } else if (pid == 0) {
      pid_t pidTlsHandler = fork();
      if (pidTlsHandler < 0) {
        std::cerr << "[ERROR] Problem initializing the service\n";
        vishnu::exitProcessOnError(-1);
      } else if (pidTlsHandler > 0) { // Parent process

        int retCode = 0;
        /* Intializing the Frontend TLS Handler */
        int sslPort = vishnu::getPortFromUri(uriAddr);
        config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivkey);
        config.getRequiredConfigValue<std::string>(vishnu::SERVER_SSL_CERTICATE, sslCertificate);
        TlsServer tlsFrontHandler(rsaPrivkey, sslCertificate, sslPort, FRONTEND_IPC_URI);
        try {
          tlsFrontHandler.run();
        } catch(VishnuException& ex) {
          std::cerr << boost::format("[ERROR] %1%\n")%ex.what();
          retCode = -1;
        } catch(...) {
          std::cerr << boost::format("[ERROR] %1%\n")%tlsFrontHandler.getErrorMsg();
          retCode = -1;
        }

        if (retCode != 0) {
          kill(pidTlsHandler, SIGKILL);
          vishnu::exitProcessOnError(retCode);
        }

        vishnu::exitProcessOnChildError(pid);

      } else if (pidTlsHandler ==  0) { // child process

        /* Intializing the Backend TLS Handler (for subscription) */
        int retCode = 0;
        int port = vishnu::getPortFromUri(uriSubs);
        TlsServer tlsBackHandler(rsaPrivkey, sslCertificate, port, BACKEND_IPC_URI);
        try {
          tlsBackHandler.run();
        } catch(VishnuException& ex) {
          std::cerr << boost::format("[ERROR] %1%\n")%ex.what();
          retCode = -1;
        } catch(...) {
          std::cerr << boost::format("[ERROR] %1%\n")%tlsBackHandler.getErrorMsg();
          retCode = -1;
        }

        vishnu::exitProcessOnError(retCode);
      }
    }
  }
}


void
Dispatcher::run() {
  readConfiguration();
  try {
    vishnu::validateUri(uriAddr);
    vishnu::validateUri(uriSubs);
    configureAnnuary();
    configureHandlers();
  } catch (const VishnuException& ex) {
    std::cerr << boost::format("[ERROR] %1%\n")%ex.what();
    exit(1);
  }
}
