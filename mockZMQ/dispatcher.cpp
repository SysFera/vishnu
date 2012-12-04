#include "Server.hpp"
#include "Annuary.hpp"
#include "utilVishnu.hpp"
#include "zhelpers.hpp"
#include "DIET_client.h"
#include <boost/thread.hpp>
#include "UserException.hpp"
#include "utils.hpp"


#define SEPARATOR "#"
#define INIT "$"

void
usage(){
  std::cout << "Usage: dispatcher <uriAddr> <uriSubscriber> [-c configFile]" << std::endl;
}

//function to get the first element from the annuary
std::string
elect(std::vector<boost::shared_ptr<Server> >* serv){
  if ((serv == NULL) || (serv->size() == 0)) {
    return "";
  }
  return serv->at(0).get()->getURI();
}

template<class Callable>
int
ZMQStartDevice(const std::string& uriExternal,
		const std::string& uriInproc,
		const int & nbWorker,
		boost::shared_ptr<Annuary>& ann) {

	// Prepare our context and the sockets for server
  boost::shared_ptr<zmq::context_t> context(new zmq::context_t(1));
  zmq::socket_t socket_server(*context, ZMQ_ROUTER);
  zmq::socket_t socket_workers(*context, ZMQ_DEALER);

  // bind the sockets
//  std::cout << boost::format("I: listening... (%1%)\n") % uriExternal;
  socket_server.bind(uriExternal.c_str());
  socket_workers.bind(uriInproc.c_str());

  // Create our threads pool
  ThreadPool pool(nbWorker);
  for (int i = 0; i < nbWorker; ++i) {
    pool.submit(Callable(context, uriInproc, i, ann));
  }

  // connect our workers threads to our server via a queue
  zmq::device(ZMQ_QUEUE, socket_server, socket_workers);

  return 0;
}

class ServiceWorker {
public:
  explicit ServiceWorker(boost::shared_ptr<zmq::context_t> ctx,
		  const std::string & uriInproc,
		  int id,
		  boost::shared_ptr<Annuary>& ann)
    : ctx_(ctx), uriInproc_(uriInproc), id_(id), mann(ann) {}

  void
  operator()() {
    Socket socket(*ctx_, ZMQ_REP);
    socket.connect(uriInproc_.c_str());

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
        std::string servname = profile.get()->name;
        std::vector<boost::shared_ptr<Server> >* serv = mann.get()->get(servname);
        std::string uriServer= elect(serv);

        std::string resultSerialized = (boost::format("error %1%: the service %2% is not available")%
        				vishnu::convertToString(ERRCODE_INVALID_PARAM)%
        				servname).str();
        if (uriServer.size() != 0) {
//            std::cout << my_serialize(profile.get());
            diet_call_gen(profile.get(), uriServer);
            resultSerialized = my_serialize(profile.get());
        }
//        std::cout << boost::format("I: Sending> %1%...\n") % resultSerialized;
        socket.send(resultSerialized);
      }
    }
  }

private:
  boost::shared_ptr<zmq::context_t> ctx_; /**< zmq context */
  std::string uriInproc_; /**< worker id */
  int id_; /**< worker id */
  boost::shared_ptr<Annuary>& mann;
};

//FIXME: clear unused parameters
class SubscripionWorker {
public:
  explicit SubscripionWorker(boost::shared_ptr<zmq::context_t> ctx,
		  const std::string & uriInproc,
		  int id,
		  boost::shared_ptr<Annuary>& ann)
    : ctx_(ctx), uriInproc_(uriInproc), id_(id), mann(ann) {}

  void
  operator()() {
    Socket socket(*ctx_, ZMQ_REP);
    socket.connect(uriInproc_.c_str());

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
//      std::cout << boost::format("I: Recv> %1%, size %2%\n") % data % data.length();

      int mode = vishnu::convertToInt(data.substr(0,1));

      // Deserialize
      boost::shared_ptr<Server> server = Server::fromString(data.substr(1));

      if (mode == 1) { // If add a server
        mann.get()->add(server.get()->getName(), server.get()->getServices(), server.get()->getURI());
      } else if (mode == 0) {
        mann.get()->remove(server.get()->getName(), server.get()->getURI());
      }
      std::string resultSerialized = "OK";

      // Send reply back to client
//      std::cout << boost::format("I: Sending> %1%...\n") % resultSerialized;
      socket.send(resultSerialized);
    }
  }

private:
  boost::shared_ptr<zmq::context_t> ctx_; /**< zmq context */
  std::string uriInproc_; /**< worker id */
  int id_; /**< worker id */
  boost::shared_ptr<Annuary>& mann;
};


class AddressDealer{
public:
  AddressDealer(std::string uri, boost::shared_ptr<Annuary>& ann, int nbThread):muri(uri), mann(ann), mnbThread(nbThread){
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
    ZMQStartDevice<ServiceWorker>(muri, "inproc://vishnuServiceWorker", mnbThread, mann);
  }


private:
  std::string muri;
  boost::shared_ptr<Annuary>& mann;
  int mnbThread;
};

class AddressSubscriber{
public:
  AddressSubscriber(std::string uri, boost::shared_ptr<Annuary>& ann, int nbThread):muri(uri), mann(ann), mnbThread(nbThread){
  }

  ~AddressSubscriber(){
  }

  void
  run(){
    ZMQStartDevice<SubscripionWorker>(muri, "inproc://vishnuSubcriberWorker", mnbThread, mann);
  }


private:
  std::string muri;
  boost::shared_ptr<Annuary>& mann;
  int mnbThread;
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

//        std::cout << boost::format("I: Sending> %1%...\n") % p1;
        if (!lpc.send(p1)) {
//          std::cout << boost::format("I: Sed Disconnected %1%\n") % uri;
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

// Strictly because optionnal argument now
  if (argc < 4){
    usage();
    return 0;
  }

  std::string uriAddr = std::string(argv[1]);
  std::string uriSubs = std::string(argv[2]);
  std::string optConf ;
  std::string confFil ;
  std::string nthread ;
  if (argc>=6){
    optConf = std::string(argv[3]);
    confFil = std::string(argv[4]);
    nthread = std::string(argv[5]);
// Optionnal parameter
    if (optConf.compare("-c")==0){
      ann.get()->initFromFile(confFil);
    }
  }


  AddressDealer AD = AddressDealer(uriAddr, ann, vishnu::convertToInt(nthread.c_str()));
  AddressSubscriber AS = AddressSubscriber(uriSubs, ann, vishnu::convertToInt(nthread.c_str()));
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
