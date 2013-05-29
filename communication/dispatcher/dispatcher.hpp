/**
 * \file dispatcher.hpp
 * \brief This file contains the dispatcher, that is a proxy to handle communication between clients and servers
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _DISPATCHER_HPP_
#define _DISPATCHER_HPP_

#include <boost/scoped_ptr.hpp>
#include <string>
#include "Annuary.hpp"
#include "handlers.hpp"
#include "ExecConfiguration.hpp"

/**
 * \class Dispatcher
 * \brief contains the main body of Dispatcher
 */
class Dispatcher {
public:
  /**
   * \brief Constructor
   * \param confFile the configuration file
   */
  Dispatcher(const std::string &confFile = "");

  /**
   * \brief Run the dispatcher in the server mode
   */
  void
  run();

private:
  /**
   * \brief Display the dispatcher configuration
   */
  void
  printConfiguration() const;

  /**
   * \brief Read the dispatcher configuration
   */
  void
  readConfiguration();

  /**
   * \brief Prepare the annuary
   */
  void
  configureAnnuary();

  /**
   * \brief Prepare the handlers
   */
  void
  configureHandlers();

  /**
   * \brief The annuary
   */
  boost::shared_ptr<Annuary> ann;
  /**
   * \brief The configuration
   */
  ExecConfiguration config;
  /**
   * \brief The handlers of the clients
   */
  boost::scoped_ptr<Handler4Clients> clientHandler;
  /**
   * \brief The handlers of the servers
   */
  boost::scoped_ptr<Handler4Servers> serverHandler;
  /**
   * \brief The uri for dealing services
   */
  std::string  uriAddr;
  /**
   * \brief The uri for subscribers
   */
  std::string  uriSubs;
  /**
   * \brief The configuration file
   */
  std::string  confFil;
  /**
   * \brief The number of threads to use
   */
  unsigned int nthread;
  /**
   * \brief The timeout of the dispatcher
   */
  unsigned int timeout;
};



#endif /* _DISPATCHER_HPP_ */
