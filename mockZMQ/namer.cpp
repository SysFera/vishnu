#include "Server.hpp"
#include "Annuary.hpp"
#include "Message.hpp"
#include "Handler.hpp"
#include "HandlerFactory.hpp"
#include "TreatmentData.hpp"
#include "utilVishnu.hpp"

#define SEPARATOR "#"
#define INIT "$"

void
usage(){
  std::cout << "Usage: namer <address> <port> <config>" << std::endl;
}

//bool
//isGoodAction(boost::shared_ptr<Message> msg) {
//  return (msg.get()->getAction() == GEAD ||
//          msg.get()->getAction() == ADSE ||
//          msg.get()->getAction() == RESE);
//}


int main(int argc, char** argv){
  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  boost::shared_ptr<Annuary> ann = boost::shared_ptr<Annuary>(new Annuary());


  if (argc < 4){
    usage();
    return 0;
  }

  std::string address = std::string(argv[1]);
  int port = atoi(argv[2]);
  std::string cfg = std::string(argv[3]);


  std::string add = address + ":" + vishnu::convertToString<int>(port);
  std::cout << "Binded to address: " << add << std::endl;
  std::cout << "config file: " << cfg << std::endl;
  socket.bind(add.c_str());

  ann.get()->initFromFile(cfg);


  while (true) {

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

    // Deserialize
    std::string servname = data;

    std::vector<boost::shared_ptr<Server> >* serv = ann.get()->get(servname);
    std::string resultSerialized;

    for (int i = 0 ; i < serv->size() ; i++){

      std::string port = vishnu::convertToString(serv->at(i).get()->getPort());
      std::string address = serv->at(i).get()->getAddress();



//    boost::shared_ptr<Message> msg(Message::fromString(data));
//    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
//    fmsserver->get()->call(profile.get());
//    HandlerFactory hf;
//    boost::shared_ptr<Message> answer;

// The mapper can only receive known actions, otherwise send an error
//    if (isGoodAction(msg)) {
//      Handler* handl = hf.getHandler(msg.get()->getHandler(), msg);
//      t.setAnnuary(ann.get());
//      answer = handl->treat(&t);
//    } else {
//      answer = boost::shared_ptr<Message>(new Message(msg.get()->getHandler(), HAER, msg.get()->getServer(), msg.get()->getProfile(), std::string("Unknown message type received by the annuary")));
//    }
//
      resultSerialized += INIT + servname + SEPARATOR + address + SEPARATOR + port;// = answer.get()->toString();
    }
    delete (serv);



    // Send reply back to client
//    std::string resultSerialized = my_serialize(profile.get());
    std::cout << " Serialized to send : " << resultSerialized << std::endl;
    zmq::message_t reply(resultSerialized.length()+1);
    memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
    socket.send(reply);
  }


}
