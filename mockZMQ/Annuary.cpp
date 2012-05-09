#include "mdcliapi.hpp"

int ZMQServerStart(string addr, int port)
{

  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  string add = addr + ":" + convertToString<int>(port);
  cout << "Binded to address: " << add << endl;
  socket.bind(add.c_str());

  while (true) {

//    std::cout << "Received a message" << std::endl;

    //Receive message from ZMQ
    zmq::message_t message(0);
    try {
      if (!socket.recv(&message, 0)) {
        return false;
      }
    } catch (zmq::error_t error) {
      std::cout << "E: " << error.what() << std::endl;
      return false;
    }
    std::string data = static_cast<const char *>(message.data());
    std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";
    // Deserialize and call UMS Method
//    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
//    fmsserver->get()->call(profile.get());


    // Send reply back to client
    std::string resultSerialized = my_serialize(profile.get());
//    std::cout << " Serialized to send : " << resultSerialized << std::endl;
    zmq::message_t reply(resultSerialized.length()+1);
    memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
    socket.send(reply);
  }

  return 0;

}


Annuary::Annuary(){
}

Annuary::~Annuary(){
  int i;
  for (i=0;i<mservers.size();++i){
    delete(mservers.at(i));
  }
}


int
Annuary::add(std::string name, std::vector<std::string> services, int port, std::string address){
  Servers* s = new Server(name, services, address, port);
  servers.push_back(s);
}


int
Annuary::remove(std::string name, int port, std::string address){
  int i;
  for (i=0;i<servers.size();++i){
    if (name.compare(servers.at(i)->getName()) == 0 &&
        address.compare(servers.at(i).getAddress) == 0 &&
        port == servers.at(i).getPort()){
      servers.erase(servers.begin()+i);
    }
  }
}


Server*
Annuary::get(std::string name){
  int i;
  for (i=0;i<servers.size();++i){
    if (name.compare(servers.at(i)->getName()) == 0){
      return servers.at(i);
    }
  }
}


Server*
Annuary::get(std::string service){
  int i;
  for (i=0;i<servers.size();++i){
    if (servers.at(i)->hasService(service)){
      return servers.at(i);
    }
  }
}



