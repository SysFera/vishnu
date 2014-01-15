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
   * \param result The result of the ping
   * \param serverName The name of the server
   * \param mid a machineId (optionnal)
   * \return 0 if everything is OK, another value otherwise
   */
  int
  ping(std::map<std::string, std::string>& result,
       const std::string& serverName = "",
       const std::string& mid = "");

  /**
   * \brief Get the version of a server
   * \param result The result of the ping
   * \param serverName The name of the server
   * \param mid a machineId (optionnal)
   * \return 0 if everything is OK, another value otherwise
   */
  int
  getVersion(std::map<std::string, std::string>& result,
             const std::string& serverName = "",
             const std::string& mid = "");
}


#endif // API_CORE_HPP
