#ifndef __MESSAGE__H__
#define __MESSAGE__H__

#include <vector>
#include <string>

#include "Server.hpp"

class Message {
private:
  std::string mhandler;
  std::string maction;
  boost::shared_ptr<Server> mserver;
  boost::shared_ptr<diet_profile_t> mprofile;


public:

  static boost::shared_ptr<Message>
  fromString(std::string);

  Message(std::string handler, std::string action, boost::shared_ptr<Server> serv, boost::shared_ptr<diet_profile_t> prof);

  ~Message();

  std::string
  getAction();

  boost::shared_ptr<Server>
  getServer();

  boost::shared_ptr<diet_profile_t>
  getProfile();

  std::string
  toString();
};

#endif // __MESSAGE__H__
