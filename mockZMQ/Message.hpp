#ifndef __MESSAGE__H__
#define __MESSAGE__H__

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

#include "DIET_client.h"
#include "Server.hpp"
#include "Handler.hpp"

class Message {
private:
  std::string mhandler; // Data on the comm handler to use
  MsgType maction; // Type of the message (action to do : give address, etc ...)
  boost::shared_ptr<Server> mserver; // The server needed
  boost::shared_ptr<diet_profile_t> mprofile; // Profile of the service on the server
  std::string mdata;

public:

  static boost::shared_ptr<Message>
  fromString(std::string);

  Message(std::string handler, MsgType action, boost::shared_ptr<Server> serv, boost::shared_ptr<diet_profile_t> prof, std::string data = "");

  ~Message();

  MsgType
  getAction();

  std::string
  getHandler();

  std::string
  getData();

  boost::shared_ptr<Server>
  getServer();

  void
  setServer(boost::shared_ptr<Server> serv);

  boost::shared_ptr<diet_profile_t>
  getProfile();

  std::string
  toString();

  void
  setData(std::string data);

  void
  setAction(MsgType ac);
};

#endif // __MESSAGE__H__
