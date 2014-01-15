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
#include <boost/shared_ptr.hpp>
#include "Server.hpp"
#include "VersionProxy.hpp"

class PingerProxy : public VersionProxy{
public :
  /**
   * \brief Default constructor
   */
  PingerProxy();
  /**
   * \brief Destructor
   */
  ~PingerProxy();
  /**
   * \brief Ping the servers on the machine and store the ping result in result
   * \param server name of server to ping, may be 'dispatcher', 'umssed', 'tmssed', 'fmssed', 'imssed'
   * \param mid the machine identifier of the machine to ping
   * \param result a map of URI / version (or failure as version if ping failed)
   */
  void
  ping(const std::string& server, const std::string mid, std::map<std::string, std::string>& result);

};


#endif // PINGER_PROXY_H
