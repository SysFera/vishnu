#include "Message.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include "utilVishnu.hpp"


boost::shared_ptr<Message>
Message::fromString(std::string prof){
  boost::shared_ptr<Message> res;
  std::vector<std::string> vecString;
  boost::algorithm::split_regex(vecString, prof, boost::regex("\\#{3}"));
  MsgType action;
  std::string handler;
  std::string server_ser;
  std::string profile_ser;
  std::string data;
  int i;
  boost::shared_ptr<Server> server;
  boost::shared_ptr<diet_profile_t> profile;


  if (!vecString.empty()) {
    std::vector<std::string>::iterator it= vecString.begin();
    handler = std::string(strdup((it++)->c_str()));
    action = MsgType(vishnu::convertToInt(std::string(strdup((it++)->c_str()))));
    server_ser = std::string(strdup((it++)->c_str()));
    server = Server::fromString(server_ser);
    profile_ser = std::string(strdup((it++)->c_str()));
    profile = my_deserialize(profile_ser);
    data = std::string(strdup((it++)->c_str()));
    res.reset(new Message(handler, action, server, profile, data));
  }
  return res;

}


Message::Message(std::string handler, MsgType action, boost::shared_ptr<Server> serv, boost::shared_ptr<diet_profile_t> prof, std::string data) :
  mhandler(handler), maction(action), mserver(serv), mprofile(prof), mdata(data) {
}


Message::~Message(){
}

MsgType
Message::getAction() {
  return maction;
}

void
Message::setAction(MsgType ac) {
  maction = ac;
}

std::string
Message::getData() {
  return mdata;
}

void
Message::setData(std::string data) {
  mdata = data;
}

std::string
Message::getHandler() {
  return mhandler;
}


boost::shared_ptr<Server>
Message::getServer(){
  return mserver;
}

void
Message::setServer(boost::shared_ptr<Server> s){
  mserver = s;
}

boost::shared_ptr<diet_profile_t>
Message::getProfile(){
  return mprofile;
}



std::string
Message::toString(){
  std::stringstream res;
  res << mhandler << "###"
      << vishnu::convertToString<int>(maction) << "###"
      << mserver.get()->toString() << "###"
      << my_serialize(mprofile.get()) << "###"
      << mdata;
  return res.str();
}

