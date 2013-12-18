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

class PingerProxy{
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
  /**
   * \brief Ping all the elements including the dispatcher
   * \param result a map of URI / version (or failure as version if ping failed)
   */
  void
  allPing(std::map<std::string, std::string>& result);

private:
  /**
   * \brief Return the longest service starting by the pattern service in the list
   */
  std::string
  getLongestService(std::vector<std::string>& list, std::string service);
  /**
   * \brief Select the servers in server that correspond to the filter on the name and the machine and store them in result
   * \param filterName filter on name
   * \param allServer list of all servers
   * \param result servers on the machine with this name
   */
  void
  selectServerToPing(std::string filterName,
                     std::vector<boost::shared_ptr<Server> >& allServers,
                     std::vector<boost::shared_ptr<Server> >& result);


};


#endif // PINGER_PROXY_H
