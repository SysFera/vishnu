#include "Server.hpp"
#include "Annuary.hpp"
#include "utilVishnu.hpp"
#include "zhelpers.hpp"
#include "DIET_client.h"
#include <boost/thread.hpp>
#include "UserException.hpp"


#define SEPARATOR "#"
#define INIT "$"

void
usage(){
  std::cout << "Usage: dispatcher <uriAddr> <uriSubscriber>" << std::endl;
}

class AddressDealer{
public:
  AddressDealer(std::string uri, boost::shared_ptr<Annuary>& ann):muri(uri), mann(ann){
  }

  ~AddressDealer(){
  }

  //function to get the first element from the annuary
  std::string
  elect(std::vector<boost::shared_ptr<Server> >* serv){
    if ((serv == NULL) || (serv->size() == 0)) {
      return "";
    }
    return serv->at(0).get()->getURI();
  }

  void
  run(){
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    std::string servname;
    std::string resultSerialized;
    std::vector<boost::shared_ptr<Server> >* serv = NULL;

    socket.bind(muri.c_str());
    std::cout << boost::format("I: listening for clients (%1%)\n") % muri;
    while (true) {

      //Receive message from ZMQ
      zmq::message_t message(0);
      try {
        if (!socket.recv(&message, 0)) {
        }
      } catch (zmq::error_t error) {
        std::cerr << boost::format("E: %1%\n")%error.what();
      }

      // Deserialize and call UMS Method
      if (message.size() != 0) {
        boost::shared_ptr<diet_profile_t> profile(my_deserialize(static_cast<const char*>(message.data())));
        servname = profile.get()->name;
        resultSerialized = (boost::format("error %1%: the service %2% is not available")%
                                        vishnu::convertToString(ERRCODE_INVALID_PARAM)%
                                        servname).str();
        try{
          serv = mann.get()->get(servname);
        } catch (UserException& e){
          diet_string_set(diet_parameter(profile.get(), profile.get()->OUT-1), strdup(e.what()), 1);
          resultSerialized = my_serialize(profile.get());
          std::cout << boost::format("IERR: Sending> %1%...\n") % resultSerialized;
          s_send(socket, resultSerialized);
          continue;
        }

        std::string uriServer= elect(serv);

        if (uriServer.size() != 0) {
            std::cout << my_serialize(profile.get());
            diet_call_gen(profile.get(), uriServer);
            resultSerialized = my_serialize(profile.get());
        }
        std::cout << boost::format("I: Sending> %1%...\n") % resultSerialized;
        s_send(socket, resultSerialized);
      }
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
    std::cout << boost::format("I: listening for SeDs subscribers (%1%)\n") % muri;
    while (true) {

      //Receive message from ZMQ
      zmq::message_t message(0);
      try {
        if (!socket.recv(&message, 0)) {
        }
      } catch (zmq::error_t error) {
        std::cerr << boost::format("E: %1%\n") % error.what();
      }
      std::string data(static_cast<const char*>(message.data()), message.size());
      std::cout << boost::format("I: Recv> %1%, size %2%\n") % data % data.length();

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
      std::cout << boost::format("I: Sending> %1%...\n") % resultSerialized;
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

        std::cout << boost::format("I: Sending> %1%...\n") % p1;
        if (!lpc.send(p1)) {
          std::cout << boost::format("I: Sed Disconnected %1%\n") % uri;
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


  if (argc != 3){
    usage();
    return 0;
  }

  std::string uriAddr = std::string(argv[1]);
  std::string uriSubs = std::string(argv[2]);


//  ann.get()->initFromFile(cfg);
  AddressDealer AD = AddressDealer(uriAddr, ann);
  AddressSubscriber AS = AddressSubscriber(uriSubs, ann);
//  Heartbeat HB = Heartbeat(20, ann);

  boost::thread th1(boost::bind(&AddressDealer::run, &AD));//%RELAX<MISRA_0_1_3> Because it used to launch a thread
  boost::thread th2(boost::bind(&AddressSubscriber::run, &AS));//%RELAX<MISRA_0_1_3> Because it used to launch a thread
//  boost::thread th3(boost::bind(&Heartbeat::run, &HB));//%RELAX<MISRA_0_1_3> Because it used to launch a thread

  // To avoid quitting too fast in case of problems
  while(1){
    sleep(1000);
  }
  return 0;
}
