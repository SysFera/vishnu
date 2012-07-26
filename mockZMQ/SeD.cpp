#include "SeD.hpp"
#include <boost/format.hpp>
#include <iostream>

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

int
ZMQServerStart(boost::shared_ptr<SeD> server,
               std::string addr, int port) {
  // Prepare our context and socket for server
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);

  std::string add = boost::str(boost::format("%1%:%2%") % addr % port);
  std::cerr << "Binded to address: " << add << "\n";
  socket.bind(add.c_str());

  while (true) {
    //Receive message from ZMQ
    zmq::message_t message(0);
    try {
      if (!socket.recv(&message, 0)) {
	return false;
      }
    } catch (zmq::error_t error) {
      std::cout << "E: " << error.what() << "\n";
      return false;
    }

    std::string data = static_cast<const char *>(message.data());
    std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";


    // Deserialize and call UMS Method
    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
    server->call(profile.get());

    // Send reply back to client
    std::string resultSerialized = my_serialize(profile.get());

    zmq::message_t reply(resultSerialized.length()+1);
    memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
    socket.send(reply);
  }
  return 0;
}
