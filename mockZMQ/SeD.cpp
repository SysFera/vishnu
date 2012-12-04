#include "SeD.hpp"
#include <iostream>
#include <boost/format.hpp>
#include "zhelpers.hpp"
#include "utils.hpp"
#include "utilVishnu.hpp"
#include "zmq.hpp"

int
heartbeat(diet_profile_t* pb){
  std::string ack("OK");
  diet_string_set(diet_parameter(pb,1), const_cast<char*>(ack.c_str()), DIET_VOLATILE);
  return 0;
}

SeD::SeD(){
  mcb.insert( std::pair<std::string, functionPtr_t> ("heartbeat", heartbeat));
}


int
SeD::call(diet_profile_t* profile) {
  std::map<std::string, functionPtr_t>::iterator it;
  it = mcb.find(profile->name);
  if (it == mcb.end()) {
    std::cerr << "E: service not found: " << profile->name << "\n";
    return UNKNOWN_SERVICE;
  }
  int (*functionPtr)(diet_profile_t*);
  functionPtr = it->second;
  return (*functionPtr)(profile);
}

std::vector<std::string>
SeD::getServices(){
  std::vector<std::string> res;
  std::map<std::string, functionPtr_t>::iterator it;
  for (it = mcb.begin() ; it != mcb.end() ; ++ it ) {
    res.push_back(it->first);
  }
  return res;
}


class ZMQWorker {
public:
  explicit ZMQWorker(boost::shared_ptr<zmq::context_t> ctx,
                     boost::shared_ptr<SeD> server, int id)
    : ctx_(ctx), server_(server), id_(id) {
  }

  void
  operator()() {
    Socket socket(*ctx_, ZMQ_REP);
    socket.connect("inproc://vishnu");
    std::string data;
    while (true) {
      data.clear();
      try {
        data = socket.get();
      } catch (zmq::error_t error) {
        std::cout << "E: " << error.what() << "\n";
        continue;
      }
      std::cout << boost::str(boost::format("Worker %1% | Recv: %2% | Size: %3%\n")% id_ % data % data.length());

      // Deserialize and call UMS Method
      if (data.size() != 0) {
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
};


int
ZMQServerStart(boost::shared_ptr<SeD> server, const std::string& uri) {

	// Prepare our context and the sockets for server
  boost::shared_ptr<zmq::context_t> context(new zmq::context_t(1));
  zmq::socket_t socket_server(*context, ZMQ_ROUTER);
  zmq::socket_t socket_workers(*context, ZMQ_DEALER);

  // bind the sockets
  std::cout << boost::format("I: listening... (%1%)\n") % uri;
  socket_server.bind(uri.c_str());
  socket_workers.bind("inproc://vishnu");

  // Create our threads pool
  const int NB_THREADS = 5;
  ThreadPool pool(NB_THREADS);
  for (int i = 0; i < NB_THREADS; ++i) {
    pool.submit(ZMQWorker(context, server, i));
  }

  // connect our workers threads to our server via a queue
  zmq::device(ZMQ_QUEUE, socket_server, socket_workers);

  return 0;
}
