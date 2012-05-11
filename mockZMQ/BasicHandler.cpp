#include "BasicHandler.hpp"

BasicHandler::BasicHandler(boost::shared_ptr<Message> msg) : Handler(msg) {
}

BasicHandler::~BasicHandler(){
}

boost::shared_ptr<Message>
BasicHandler::treat(TreatmentData* data){
  boost::shared_ptr<Message> answer;
  switch (mmsg.get()->getAction()) {
  case GEAD:
    answer = getAddress(data->getAnnuary());
    break;
  case ACDO:
    answer = actionDone();
    break;
  case DOSE:
    answer = doService(data->getServer());
    break;
  case ADSE:
    answer = addServer(data->getAnnuary());
    break;
  case RESE:
    answer = removeServer(data->getAnnuary());
    break;
  default:
    answer.get()->setAction(HAER);
    answer.get()->setData("Unknown action required");
    break;
  }
  return answer;
}

boost::shared_ptr<Message>
BasicHandler::getAddress(Annuary* ann){
  boost::shared_ptr<Message> answer;
  boost::shared_ptr<Server> server = ann->get(std::string(mmsg.get()->getProfile().get()->name));
// If no server with the service found in the annuary
  if (server.use_count()==0) {
    answer.get()->setAction(HAER);
    answer.get()->setData("No server with the corresponding service found");
  }
  answer.get()->setServer(server);
  return answer;
}

boost::shared_ptr<Message>
BasicHandler::actionDone(){
  boost::shared_ptr<Message> answer = mmsg;
  return answer;
}

template <class T>
boost::shared_ptr<Message>
BasicHandler::doService(T* server){
  boost::shared_ptr<Message> answer;
  server->call(mmsg.get()->getProfile().get());
  answer = mmsg;
}

//boost::shared_ptr<Message>
//BasicHandler::actionDone(){
//  boost::shared_ptr<Message> answer = mmsg;
//  return answer;
//}

boost::shared_ptr<Message>
BasicHandler::addServer(Annuary* ann){
  boost::shared_ptr<Message> answer = mmsg;
  ann->add(mmsg.get()->getServer().get()->getName(),
           mmsg.get()->getServer().get()->getServices(),
           mmsg.get()->getServer().get()->getPort(),
           mmsg.get()->getServer().get()->getAddress());
  answer.get()->setAction(ACDO);
  return answer;
}

boost::shared_ptr<Message>
BasicHandler::removeServer(Annuary* ann){
  boost::shared_ptr<Message> answer;
  ann->remove(mmsg.get()->getServer().get()->getName(),
              mmsg.get()->getServer().get()->getPort(),
              mmsg.get()->getServer().get()->getAddress());
  answer.get()->setAction(ACDO);
  return answer;
}

