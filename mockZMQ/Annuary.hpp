#ifndef __ANNUARY__H__
#define __ANNUARY__H__

#include "Server.hpp"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

class Annuary {

public :
  Annuary();
  ~Annuary();

  int
  add(std::string name, std::vector<std::string> services, int port, std::string address);

  int
  remove(std::string name, int port, std::string address);

  boost::shared_ptr<Server>
  get(std::string service);

private :
  std::vector<boost::shared_ptr<Server> > mservers;
  int pos;

};

#endif // __ANNUARY__H__
