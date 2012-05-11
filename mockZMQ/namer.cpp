#include "Server.hpp"
#include "Annuary.hpp"

void
usage(){
  std::cout << "Usage: namer <address> <port>" << std::endl;
}

bool
isGoodAction(boost::shared_ptr<Message> msg) {
  return (msg.get().getAction() == GEAD ||
          msg.get().getAction() == ADSE ||
          msg.get().getAction() == RESE);

  if (!res) {
  }
  return res;
}


int main(int argc, char** argv){
  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  boost::shared_ptr<Annuary> ann;


  if (argc < 3){
    usage();
    return 0;
  }

  std::string address = std::string(argv[1]);
  int port = atoi(argv[2]);

  std::string add = address + ":" + convertToString<int>(port);
  cout << "Binded to address: " << add << endl;
  socket.bind(add.c_str());

  while (true) {
// Recreate a data
    TreatmentData t;


    //Receive message from ZMQ
    zmq::message_t message(0);
    try {
      if (!socket.recv(&message, 0)) {
        return 0;
      }
    } catch (zmq::error_t error) {
      std::cout << "E: " << error.what() << std::endl;
      return 0;
    }
    std::string data = static_cast<const char *>(message.data());
    std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";
    // Deserialize and call UMS Method
    boost::shared_ptr<Message> msg(Message::fromString(data));
//    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
//    fmsserver->get()->call(profile.get());
    HandlerFactory hf;
    boost::shared_ptr<Message> answer;

// The mapper can only receive known actions, otherwise send an error
    if (isGoodAction(msg)) {
      Handler* handl = hf->getHandler(msg.get().getHandler(), msg);
      t.setAnnuary(ann.get())
      answer = handl->treat(&t);
    } else {
      answer = boost::shared_ptr<Message>(Message(msg.get().getHandler(), HAER, msg.get().getServer(), msg.get().getProfile(), "Unknown message type received by the annuary"));
    }

    std::string resultSerialized = answer.get().toString();


    // Send reply back to client
//    std::string resultSerialized = my_serialize(profile.get());
    std::cout << " Serialized to send : " << resultSerialized << std::endl;
    zmq::message_t reply(resultSerialized.length()+1);
    memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
    socket.send(reply);
  }


}
