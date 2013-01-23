#ifndef _DISPATCHER_HPP_
#define _DISPATCHER_HPP_

#include <boost/scoped_ptr.hpp>
#include "Annuary.hpp"
#include "handlers.hpp"
#include "ExecConfiguration.hpp"

/**
 * @class contains the main body of Dispatcher
 */
class Dispatcher {
public:
  Dispatcher(int argc, char* argv[]);

  void
  run();

private:
  void
  printConfiguration() const;

  void
  readConfiguration();

  void
  configureAnnuary();

  void
  configureHandlers();

  boost::shared_ptr<Annuary> ann;
  ExecConfiguration config;
  boost::scoped_ptr<ClientHandler> clientHandler;
  boost::scoped_ptr<ServerHandler> serverHandler;
  std::string  uriAddr;
  std::string  uriSubs;
  std::string  confFil;
  unsigned int nthread;
  unsigned int timeout;
  int argc;
  char** argv;
};



#endif /* _DISPATCHER_HPP_ */
