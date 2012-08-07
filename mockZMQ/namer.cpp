#include "Server.hpp"
#include "Annuary.hpp"
#include "Message.hpp"
//#include "Handler.hpp"
//#include "HandlerFactory.hpp"
//#include "TreatmentData.hpp"
#include "utilVishnu.hpp"

#include <boost/thread.hpp>

#define SEPARATOR "#"
#define INIT "$"

void
usage(){
  std::cout << "Usage: namer <uriAddr> <uriSubscriber> <config>" << std::endl;
}

//bool
//isGoodAction(boost::shared_ptr<Message> msg) {
//  return (msg.get()->getAction() == GEAD ||
//          msg.get()->getAction() == ADSE ||
//          msg.get()->getAction() == RESE);
//}

class AddressDealer{
public:
  AddressDealer(std::string uri, boost::shared_ptr<Annuary>& ann):muri(uri), mann(ann){
  }

  ~AddressDealer(){
  }

  void
  run(){
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind(muri.c_str());
    std::cout << "binded " << muri << std::endl;
    while (true) {

      //Receive message from ZMQ
      zmq::message_t message(0);
      try {
        if (!socket.recv(&message, 0)) {
        }
      } catch (zmq::error_t error) {
        std::cout << "E: " << error.what() << std::endl;
      }
      std::string data(static_cast<const char*>(message.data()), message.size());
      std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";

      // Deserialize
      std::string servname = data;

      std::vector<boost::shared_ptr<Server> >* serv = mann.get()->get(servname);
      std::string resultSerialized;

      for (int i = 0 ; i < serv->size() ; i++){
        std::string uri = serv->at(i).get()->getURI();
        resultSerialized += INIT + servname + SEPARATOR + uri ;
      }
      delete (serv);

    // Send reply back to client
      std::cout << " Serialized to send : " << resultSerialized << std::endl;
      zmq::message_t reply(resultSerialized.length()+1);
      memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
      socket.send(reply);
    }
  }


private:
  std::string muri;
  boost::shared_ptr<Annuary>& mann;
};

class AddressSubscriber{
public:
  AddressSubscriber(std::string uri, boost::shared_ptr<Annuary>& ann):muri(uri), mann(ann){
  }

  ~AddressSubscriber(){
  }

  void
  run(){
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind(muri.c_str());
    std::cout << "binded subs " << muri << std::endl;
    while (true) {

      //Receive message from ZMQ
      zmq::message_t message(0);
      try {
        if (!socket.recv(&message, 0)) {
        }
      } catch (zmq::error_t error) {
        std::cout << "E: " << error.what() << std::endl;
      }
      std::string data(static_cast<const char*>(message.data()), message.size());
      std::cerr << "recv: \"" << data << "\", size " << data.length() << "\n";

      // Deserialize
      boost::shared_ptr<Server> server = Server::fromString(data);

      mann.get()->add(server.get()->getName(), server.get()->getServices(), server.get()->getURI());


      std::string resultSerialized = "OK";


    // Send reply back to client
      std::cout << " Serialized to send : " << resultSerialized << std::endl;
      zmq::message_t reply(resultSerialized.length()+1);
      memcpy(reply.data(), resultSerialized.c_str(), resultSerialized.length()+1);
      socket.send(reply);
    }
  }


private:
  std::string muri;
  boost::shared_ptr<Annuary>& mann;
};


int main(int argc, char** argv){
  // Prepare our context and socket
  boost::shared_ptr<Annuary> ann = boost::shared_ptr<Annuary>(new Annuary());


  if (argc < 4){
    usage();
    return 0;
  }

  std::string uriAddr = std::string(argv[1]);
  std::string uriSubs = std::string(argv[2]);
  std::string cfg = std::string(argv[3]);


//  ann.get()->initFromFile(cfg);
  AddressDealer AD = AddressDealer(uriAddr, ann);
  AddressSubscriber AS = AddressSubscriber(uriSubs, ann);

  boost::thread th1(boost::bind(&AddressDealer::run, &AD));//%RELAX<MISRA_0_1_3> Because it used to launch a thread
  boost::thread th2(boost::bind(&AddressSubscriber::run, &AS));//%RELAX<MISRA_0_1_3> Because it used to launch a thread

  // To avoid quitting to fast in case of problems
  while(1){
    sleep(1000);
  }
  return 0;
}
