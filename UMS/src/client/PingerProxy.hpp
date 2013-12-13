/**
 * \file PingerProxy.hpp
 * \brief This file contains the VISHNU PingerProxy class.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */
#ifndef _PINGER_PROXY_H_
#define _PINGER_PROXY_H_

#include <vector>
#include <string>

class PingerProxy{
public :
  PingerProxy();
  ~PingerProxy();
  std::string
  ping(const std::string& server, const std::string mid);
  void
  allPing(std::map<std::string, std::string>& result);

private:
  std::string
  getLongestService(std::vector<std::string>& list, std::string service);

};


#endif // PINGER_PROXY_H
