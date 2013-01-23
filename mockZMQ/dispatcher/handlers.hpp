#ifndef _HANDLERS_HPP_
#define _HANDLERS_HPP_

#include "workers.hpp"


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
    // Prepare our context and the sockets for server
    boost::shared_ptr<zmq::context_t> context(new zmq::context_t(1));
    zmq::socket_t socket_server(*context, ZMQ_ROUTER);
    zmq::socket_t socket_workers(*context, ZMQ_DEALER);

    std::string internalEndpoint = InternalEndpoint<Worker>::value();
    // bind the sockets
    socket_server.bind(muri.c_str());
    socket_workers.bind(internalEndpoint.c_str());

    // Create our threads pool
    ThreadPool pool(nbThread);
    for (int i = 0; i < nbThread; ++i) {
      pool.submit(Worker(context, internalEndpoint, i, mann));
    }

    // connect our workers threads to our server via a queue
    zmq::device(ZMQ_QUEUE, socket_server, socket_workers);
  }

private:
  const std::string muri;
  boost::shared_ptr<Annuary> mann;
  int nbThread;
};


template class Handler<ServiceWorker>;
typedef Handler<ServiceWorker> ClientHandler;
template class Handler<SubscriptionWorker>;
typedef Handler<SubscriptionWorker> ServerHandler;

#endif /* _HANDLERS_HPP_ */
