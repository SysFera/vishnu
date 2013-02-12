#ifndef _HANDLERS_HPP_
#define _HANDLERS_HPP_

#include "zhelpers.hpp"
#include "AnnuaryWorker.hpp"


template<typename T>
struct InternalEndpoint {
  static std::string value() { return ""; };
};

template<>
struct InternalEndpoint<SubscriptionWorker> {
  static std::string value() { return "inproc://vishnuSubcriberWorker"; };
};

template<>
struct InternalEndpoint<ServiceWorker> {
  static std::string value() { return "inproc://vishnuServiceWorker"; };
};


/**
 * @class generic handler parameterized by its Worker class
 */
template<typename Worker>
class Handler {
public:
  Handler(const std::string& uri, boost::shared_ptr<Annuary> ann, int nbThread)
    : muri(uri), mann(ann), nbThread(nbThread) {}

  void
  run() {
    serverWorkerSockets<Worker,
                        boost::shared_ptr<Annuary> >(muri,
                                                     InternalEndpoint<Worker>::value(),
                                                     nbThread,
                                                     mann);
  }

private:
  const std::string muri;
  boost::shared_ptr<Annuary> mann;
  int nbThread;
};


typedef Handler<ServiceWorker> ClientHandler;
typedef Handler<SubscriptionWorker> ServerHandler;

#endif /* _HANDLERS_HPP_ */
