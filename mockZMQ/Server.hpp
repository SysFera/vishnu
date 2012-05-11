#ifndef __SERVER__H__
#define __SERVER__H__

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class Server {
private:
  std::string mname;
  std::vector<std::string> mservices;
  std::string maddress;
  int mport;

public:

  static boost::shared_ptr<Server>
  fromString(std::string);

  Server(std::string name, std::vector<std::string> &serv, std::string addr, int port);

  ~Server();

  int
  add(std::string service);

  int
  remove(std::string service);

  bool
  hasService(std::string service);

  std::string
  getName();

  int
  getPort();

  std::vector<std::string>&
  getServices();

  std::string
  getAddress();

  std::string
  toString();
};

#endif // __SERVER__H__
