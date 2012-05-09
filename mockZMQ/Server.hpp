#ifndef __SERVER__H__
#define __SERVER__H__

#include <vector>
#include <string>

class Server {
private:
  std::string mname;
  std::vector<std::string> mservices;
  std::string maddress;
  int mport;

public:

  static boost::shared_ptr<Server>
  fromString(std::string);

  Server(int name, std::vector<std::string> &serv, std::string addr, int port);

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

  std::string
  getAddress();

  std::string
  toString();
};

#endif // __SERVER__H__
