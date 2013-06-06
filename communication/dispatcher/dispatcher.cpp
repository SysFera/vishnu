#include "dispatcher.hpp"
#include "Server.hpp"
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>


Dispatcher::Dispatcher(const std::string &confFile)
  : uriAddr("tcp://127.0.0.1:5560"),
    uriSubs("tcp://127.0.0.1:5561"), nthread(5), timeout(10) {
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

  // Get initial configuration
  std::vector<std::string> cfgInfo;
  if (config.getConfigValues(vishnu::FMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("FMS", cfgInfo);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::IMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("IMS", cfgInfo);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::TMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("TMS", cfgInfo);
    cfgInfo.clear();
  }
  if (config.getConfigValues(vishnu::UMS_URIADDR, cfgInfo)) {
    ann->setInitConfig("UMS", cfgInfo);
    cfgInfo.clear();
  }
  ann->print();
}


void
Dispatcher::configureHandlers() {
  std::string rsaPrivKey;
  bool useSsl = false;
  if (config.getConfigValue<bool>(vishnu::USE_SSL, useSsl) &&  useSsl) {
    config.getRequiredConfigValue<std::string>(vishnu::SERVER_PRIVATE_KEY, rsaPrivKey);
  }

  clientHandler.reset(new Handler4Clients(uriAddr, ann, nthread));
  serverHandler.reset(new Handler4Servers(uriSubs, ann, nthread));

  boost::thread th1(boost::bind(&Handler4Clients::run, clientHandler.get()));
  boost::thread th2(boost::bind(&Handler4Servers::run, serverHandler.get()));

  th1.join();
  th2.join();
}


void
Dispatcher::run() {
  readConfiguration();
  configureAnnuary();
  configureHandlers();
}
