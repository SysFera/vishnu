/**
 * \file handlers.hpp
 * \brief This file contains the definition of the handlers used for the communication layers
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _HANDLERS_HPP_
#define _HANDLERS_HPP_

#include "zhelpers.hpp"
#include "AnnuaryWorker.hpp"


/**
 * \struct InternalEndPoint
 * \brief Get the endpoint for the subscribers
 */
template<typename T>
struct InternalEndpoint {
  /**
   * \brief Return the value of the endpoint
   */
  static std::string value() { return ""; };
};

template<>
struct InternalEndpoint<SubscriptionWorker> {
  /**
   * \brief Return the value of the endpoint
   */
  static std::string value() { return "inproc://vishnuSubcriberWorker"; };
};

template<>
struct InternalEndpoint<ServiceWorker> {
  /**
   * \brief Return the value of the endpoint
   */
  static std::string value() { return "inproc://vishnuServiceWorker"; };
};


/**
 * \class Handler
 * \brief generic handler parameterized by its Worker class
 */
template<typename Worker>
class Handler {
public:

  /**
   * \brief Constructor
   * \param uri the uri for the handler
   * \param ann the annuary
   * \param nbThread the number of threads to use
   */
  Handler(const std::string& uri,
          boost::shared_ptr<Annuary> ann,
          int nbThread,
          bool usessl,
          const std::string& certCaFile)
    : muri(uri), mann(ann), nbThread(nbThread), useSsl(usessl), cafile(certCaFile) {}

  /**
   * \brief To run the handler
   */
  void
  run() {
    serverWorkerSockets<Worker,
        boost::shared_ptr<Annuary> >(muri,
                                     InternalEndpoint<Worker>::value(),
                                     nbThread,
                                     mann,
                                     useSsl,
                                     cafile);
  }

private:
  /**
   * \brief The uri
   */
  const std::string muri;

  /**
   * \brief The annuary
   */
  boost::shared_ptr<Annuary> mann;

  /**
   * \brief The number of threads to use
   */
  int nbThread;

  /**
   * \brief  path to the CA file
  */
  bool useSsl;

  /**
   * \brief  path to the CA file
  */
  std::string cafile;
};

/**
 * \brief Handler for the clients
 */
typedef Handler<ServiceWorker> Handler4Clients;
/**
 * \brief Handler for the servers
 */
typedef Handler<SubscriptionWorker> Handler4Servers;

#endif /* _HANDLERS_HPP_ */
