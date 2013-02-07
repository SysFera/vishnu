#include "SeD.hpp"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include "zhelpers.hpp"
#include "utils.hpp"
#include "utilVishnu.hpp"
#include "zmq.hpp"



int
heartbeat(diet_profile_t* pb){
  diet_string_set(pb, 1, "OK");
  return 0;
}

SeD::SeD() {
  mcb["heartbeat"] = boost::ref(heartbeat);
}


int
SeD::call(diet_profile_t* profile) {
  CallbackMap::iterator it  = mcb.find(profile->name);
  if (it == mcb.end()) {
    std::cerr << boost::format("E: service not found: %1%\n") % profile->name;
    return UNKNOWN_SERVICE;
  }
  CallbackFn fn = boost::ref(it->second);

  /* we need to catch all exceptions to prevent the SeD from
   * crashing in case the function raises an exception
   */
  int rv;
  try {
    rv = fn(profile);
  } catch (const std::exception &e) {
    rv = INTERNAL_ERROR;
    std::cerr << boost::format("%1%\n") % e.what();
  }

  return rv;
}

std::vector<std::string>
SeD::getServices() {
  std::vector<std::string> res;
  std::transform(mcb.begin(), mcb.end(), std::back_inserter(res),
                 boost::bind(&CallbackMap::value_type::first, _1));

  return res;
}


class ZMQWorker {
public:
  explicit ZMQWorker(boost::shared_ptr<zmq::context_t> ctx,
                     boost::shared_ptr<SeD> server, int id, const std::string& queue)
    : ctx_(ctx), server_(server), id_(id),  queue_(queue) {
  }

  void
  operator()() {
    Socket socket(*ctx_, ZMQ_REP);
    socket.connect(queue_.c_str());
    std::string data;
    while (true) {
        data.clear();
        try {
          data = socket.get();
        } catch (zmq::error_t &error) {
          std::cerr << boost::format("E: %1%\n") % error.what();
          continue;
        }
        std::cout << boost::format("Worker %1% | Recv: %2% | Size: %3%\n")% id_ % data % data.length();
        // Deserialize and call UMS Method
        if (!data.empty()) {
            boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
            server_->call(profile.get());
            // Send reply back to client
            std::string resultSerialized = my_serialize(profile.get());
            socket.send(resultSerialized);
          }
      }
  }

private:
  boost::shared_ptr<zmq::context_t> ctx_; /**< zmq context */
  boost::shared_ptr<SeD> server_; /**< Server implementation */
  int id_; /**< worker id */
  std::string queue_;
};


int
ZMQServerStart(boost::shared_ptr<SeD> server, const std::string& uri) {
  // Prepare our context and the sockets for server
  const std::string WORKER_INPROC_QUEUE = "inproc://vishnu-sedworkers";
  boost::shared_ptr<zmq::context_t> context(new zmq::context_t(1));
  zmq::socket_t socket_server(*context, ZMQ_ROUTER);
  zmq::socket_t socket_workers(*context, ZMQ_DEALER);

  // bind the sockets
  socket_server.bind(uri.c_str());
  socket_workers.bind(WORKER_INPROC_QUEUE.c_str());
  std::cout << boost::format("I: listening... (%1%)\n") % uri;

  // Create our threads pool
  const int NB_THREADS = 50;
  ThreadPool pool(NB_THREADS);
  for (int i = 0; i < NB_THREADS; ++i) {
      pool.submit(ZMQWorker(context, server, i, WORKER_INPROC_QUEUE));
    }

  // connect our workers threads to our server via a queue
  do {
    try {
      zmq::device(ZMQ_QUEUE, socket_server, socket_workers);
      break;
    } catch (const zmq::error_t& e) {
      if (EINTR == e.num()) {
        continue;
      } else {
        std::cerr << boost::format("E: zmq device creation failed (%1%)\n") % e.what();
        return 1;
      }
    }
  } while(true);

  return 0;
}
