/**
 * \file VersionProxy.hpp
 * \brief This file contains the VISHNU VersionProxy class.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */
#ifndef _VERSION_PROXY_H_
#define _VERSION_PROXY_H_

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Server.hpp"

class VersionProxy{
public :
  /**
   * \brief Default constructor
   */
  VersionProxy();
  /**
   * \brief Destructor
   */
  ~VersionProxy();
  /**
   * \brief Ping the servers on the machine and store the ping result in result
   * \param server name of server to ping, may be 'dispatcher' or 'xmssed'
   * \param mid the machine identifier of the machine to ping
   * \param result a map of URI / version (or failure as version if ping failed)
   */
  void
  getVersion(const std::string& server, const std::string mid, std::map<std::string, std::string>& result);
  /**
   * \brief List the servers in the dispatcher
   * \param mid the machine identifier of the machine to ping
   * \param result a map of server / version
   */
  void
  list(const std::string mid, std::map<std::string, std::string>& result);

protected:
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


#endif // VERSION_PROXY_H
