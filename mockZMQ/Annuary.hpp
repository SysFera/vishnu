#ifndef __ANNUARY__H__
#define __ANNUARY__H__

#include "Server.hpp"

class Annuary {

public :
  Annuary();
  ~Annuary();

  int
  add(std::string name, std::vector<std::string> services, int port, std::string address);

  int
  remove(std::string name, int port, std::string address);

  Server*
  get(std::string name);

  Server*
  get(std::string service);

private :
  std::vector<Server*> mservers;
  int pos;

};

#endif // __ANNUARY__H__
