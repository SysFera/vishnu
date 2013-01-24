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
  clientHandler.reset(new ClientHandler(uriAddr, ann, nthread));
  serverHandler.reset(new ServerHandler(uriSubs, ann, nthread));

  boost::thread th1(boost::bind(&ClientHandler::run,
                                clientHandler.get()));
  boost::thread th2(boost::bind(&ServerHandler::run,
                                serverHandler.get()));
}


void
Dispatcher::run() {
  readConfiguration();
  configureAnnuary();
  configureHandlers();
}
