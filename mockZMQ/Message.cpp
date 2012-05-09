#include "Message.hpp"

static boost::shared_ptr<Message>
Message::fromString(std::string){
  boost::shared_ptr<Message> res;
  std::vector<std::string> vecString;
  boost::algorithm::split_regex(vecString, prof, boost::regex("\\#{3}"));
  std::string action;
  std::string handler;
  std::string server_ser;
  std::string prof_ser;
  int i;
  boost::shared_ptr<Server> server;
  boost::shared_ptr<diet_profile_t> profile;


  if (!vecString.empty()) {
    std::vector<std::string>::iterator it= vecString.begin();
    handler = std::string(strdup((it++)->c_str()));
    action = std::string(strdup((it++)->c_str()));
    server_ser = std::string(strdup((it++)->c_str()));
    server = Server::fromString(server_ser);
    profile_ser = std::string(strdup((it++)->c_str()));
    profile = my_deserialize(profile_ser);
    res.reset(new Message(handler, action, server, profile));
  }
  return res;

}


Message::Message(std::string handler, std::string action, boost::shared_ptr<Server> serv, boost::shared_ptr<diet_profile_t> prof) :
  maction(action), mserver(serv) {}


Message::~Message();

std::string
Message::getAction() {
  return maction;
}


boost::shared_ptr<Server>
Message::getServer(){
  return mserver;
}

boost::shared_ptr<diet_profile_t>
Message::getProfile(){
  return mprofile;
}



std::string
Message::toString(){
  std::stringstream res;
  res << mhandler << "###"
      << maction << "###"
      << mserver.get()->toString() << "###"
      << my_serialize(mprofile);
  return res.str();
}

