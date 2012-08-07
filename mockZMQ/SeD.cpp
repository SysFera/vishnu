#include "SeD.hpp"
#include <iostream>
#include <boost/format.hpp>
#include "utils.hpp"

int
SeD::call(diet_profile_t* profile) {
  std::map<std::string, functionPtr_t>::iterator it;
  it = mcb.find(profile->name);
  if (it == mcb.end()) {
    std::cerr << "not found service : " << profile->name << "\n";
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
    zmq::socket_t socket(*ctx_, ZMQ_REP);
    socket.connect("inproc://vishnu");
    while (true) {
      //Receive message from ZMQ
      zmq::message_t message(0);
      try {
        socket.recv(&message, 0);
      } catch (zmq::error_t error) {
        std::cout << "E: " << error.what() << "\n";
      }

      std::string data = static_cast<const char *>(message.data());
      std::cerr << boost::str(
        boost::format("ZMQ Worker: %|1$02| |"
                      "recv: %2% | size: %3%\n")
        % id_ % data % data.length());


      // Deserialize and call UMS Method
      boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
      server_->call(profile.get());

      // Send reply back to client
      std::string resultSerialized = my_serialize(profile.get());

      zmq::message_t reply(resultSerialized.length()+1);
      memcpy(reply.data(), resultSerialized.c_str(),
             resultSerialized.length()+1);
      socket.send(reply);
    }
  }

private:
  boost::shared_ptr<zmq::context_t> ctx_; /**< zmq context */
  boost::shared_ptr<SeD> server_; /**< Server implementation */
  int id_; /**< worker id */
};


int
ZMQServerStart(boost::shared_ptr<SeD> server, const std::string& uri) {
  // Prepare our context and socket for server
  boost::shared_ptr<zmq::context_t> context(new zmq::context_t(1));
  zmq::socket_t socket_server(*context, ZMQ_ROUTER);
  zmq::socket_t socket_workers(*context, ZMQ_DEALER);
  // bind our sockets
  std::cerr << "Binded to address: " << uri << "\n";
  socket_server.bind(uri.c_str());
  socket_workers.bind("inproc://vishnu");

  // Create our threads pool
  ThreadPool pool(5);
  for (int i = 0; i < 5; ++i) {
    pool.submit(ZMQWorker(context, server, i));
  }

  // boost::thread t(ZMQWorker(context, server, 0));
  // connect our workers threads to our server via a queue
  zmq::device(ZMQ_QUEUE, socket_server, socket_workers);

  return 0;
}
