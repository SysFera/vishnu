#include "SeD.hpp"

#include <iostream>
#include <algorithm>                    // for transform
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <iterator>                     // for back_insert_iterator, etc
#include <string>
#include <utility>                      // for pair

#include "Worker.hpp"                   // for serverWorkerSockets
#include "zhelpers.hpp"
#include "zmq.hpp"
#include "SeDWorker.hpp"
#include "VishnuException.hpp"
#include "vishnu_version.hpp"


int
heartbeat(diet_profile_t* pb){
  std::string serviceName = std::string(pb->name);
  std::string msg;
  std::string batch;
  if (serviceName.find("tms") != std::string::npos){
    batch = (boost::format("\n%1% %2%")%VISHNU_BATCH_SCHEDULER%VISHNU_BATCH_SCHEDULER_VERSION).str();
  }
  msg = (boost::format("%1% %2%")%VISHNU_VERSION%batch).str();

  diet_string_set(pb, 1, msg);
  std::cout << "heartbeating " << std::endl;
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
// To show it is an invalid profile
    profile->OUT = -1;
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
    throw SystemException(ERRCODE_INVDATA, e.what());
    std::cerr << boost::format("[ERROR] %1%\n") % e.what();
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

      // Deserialize and call UMS Method
      if (!data.empty()) {
        try {
          boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
          server_->call(profile.get()); //FIXME: deal with possibly error
          std::string resultSerialized = my_serialize(profile.get());
          socket.send(resultSerialized);
        } catch (const VishnuException& ex) {
          socket.send(ex.what());
          std::cerr << boost::format("[ERROR] %1%\n")%ex.what();
        }
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

/**
 * @brief ZMQServerStart
 * @param server
 * @param uri
 * @param useSsl
 * @param cafile
 * @return
 */
int
ZMQServerStart(boost::shared_ptr<SeD> server,
               const std::string& uri,
               bool useSsl,
               const std::string& cafile) {
  const std::string WORKER_INPROC_QUEUE = "inproc://vishnu-sedworkers";
  const int NB_THREADS = 5;

  return serverWorkerSockets<SeDWorker,
      boost::shared_ptr<SeD> >(uri,
                               WORKER_INPROC_QUEUE,
                               NB_THREADS,
                               server,
                               useSsl,
                               cafile);
}
