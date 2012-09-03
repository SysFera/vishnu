#include "Server.hpp"
#include "Annuary.hpp"
#include "utilVishnu.hpp"
#include "zhelpers.hpp"
#include "DIET_client.h"
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
      s_send(socket, resultSerialized) ;
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

      int mode = vishnu::convertToInt(data.substr(0,1));


      // Deserialize
      boost::shared_ptr<Server> server = Server::fromString(data.substr(1));

      if(mode==1) { // If add a server
        mann.get()->add(server.get()->getName(), server.get()->getServices(), server.get()->getURI());
      } else if (mode == 0) {
        mann.get()->remove(server.get()->getName(), server.get()->getURI());
      }
      std::string resultSerialized = "OK";

      // Send reply back to client
      std::cout << " Serialized to send : " << resultSerialized << std::endl;
      s_send(socket, resultSerialized);
    }
  }


private:
  std::string muri;
  boost::shared_ptr<Annuary>& mann;
};

class Heartbeat{
public:
  Heartbeat(int freq, boost::shared_ptr<Annuary>& ann):mfreq(freq), mann(ann){
  }

  ~Heartbeat(){
  }


  void
  run(){
    zmq::context_t ctxt (1);
    std::vector<boost::shared_ptr<Server> >::iterator it;
    while (true) {
      std::vector<boost::shared_ptr<Server> >* serv = mann->get("");
      for(it = serv->begin(); it != serv->end() ; ++it){
        std::string uri = it->get()->getURI();
        LazyPirateClient lpc(ctxt, uri);
        diet_profile_t* hb = NULL;
        hb = diet_profile_alloc("heartbeat", 0, 0, 1);
        //OUT Parameters
        diet_string_set(diet_parameter(hb,0), const_cast<char *>("hb"), DIET_VOLATILE);
        diet_string_set(diet_parameter(hb,1), NULL, DIET_VOLATILE);
        std::string p1 = my_serialize(hb);

        if (!lpc.send(p1)) {
          std::cerr << "Deconnexion of "+uri;
          mann->remove(it->get()->getName(), it->get()->getURI());
        }
      }
      sleep(mfreq);
    }
  }


private:
  int mfreq;
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
//  Heartbeat HB = Heartbeat(20, ann);

  boost::thread th1(boost::bind(&AddressDealer::run, &AD));//%RELAX<MISRA_0_1_3> Because it used to launch a thread
  boost::thread th2(boost::bind(&AddressSubscriber::run, &AS));//%RELAX<MISRA_0_1_3> Because it used to launch a thread
//  boost::thread th3(boost::bind(&Heartbeat::run, &HB));//%RELAX<MISRA_0_1_3> Because it used to launch a thread

  // To avoid quitting to fast in case of problems
  while(1){
    sleep(1000);
  }
  return 0;
}
