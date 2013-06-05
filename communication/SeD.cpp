#include "SeD.hpp"

#include <iostream>
#include <algorithm>                    // for transform
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <iterator>                     // for back_insert_iterator, etc
#include <utility>                      // for pair

#include "Worker.hpp"                   // for serverWorkerSockets
#include "zhelpers.hpp"
#include "zmq.hpp"
#include "SeDWorker.hpp"


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
                     boost::shared_ptr<SeD> server,
                     int id,
                     const std::string& queue)
    : ctx_(ctx),
      server_(server),
      id_(id),
      queue_(queue)
  {
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

  /**
   * @brief zmq context
   */
  boost::shared_ptr<zmq::context_t> ctx_;

  /**
   * @brief server_
   */
  boost::shared_ptr<SeD> server_;

  /**
   * @brief the worker id
   */
  int id_;

  /**
   * @brief queue_
   */
  std::string queue_;
};


int
ZMQServerStart(boost::shared_ptr<SeD> server,
               const std::string& uri) {
  const std::string WORKER_INPROC_QUEUE = "inproc://vishnu-sedworkers";
  const int NB_THREADS = 5;

  return serverWorkerSockets<SeDWorker,
                             boost::shared_ptr<SeD> >(uri,
                                                      WORKER_INPROC_QUEUE,
                                                      NB_THREADS,
                                                      server);
}
