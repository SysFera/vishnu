/** \file api_comm.hpp
 * \brief This file contains the VISHNU api functions for the core package.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date December 2013
 */

#ifndef API_CORE_HPP
#define API_CORE_HPP

#include <map>
#include <string>


namespace vishnu {

  /**
   * \brief create a ping service
   * \param serverName The name of the server
   * \param result The result of the ping
   * \param mid a machineId (optionnal)
   * \return 0 if everything is OK, another value otherwise
   */
  int
  ping(const std::string& serverName,
       std::map<std::string, std::string>& result,
       const std::string& mid = "");

  /**
   * \brief Ping all the servers
   * \param result A map of service and their version
   * \return 0 if everything is OK, another value otherwise
   */
  int
  allPing(std::map<std::string, std::string>& result);
}


#endif // API_CORE_HPP
