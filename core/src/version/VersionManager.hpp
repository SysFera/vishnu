/**
 * \file VersionManager.hpp
 * \brief This file defines the version manager object
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 20/08/12
 */

#ifndef _VERSION_MANAGER_H_
#define _VERSION_MANAGER_H_

#include <iostream>
#include "Version.hpp"

using namespace std;

/**
 * \class VersionManager
 * \brief VersionManager
 */
class VersionManager {

public :
  /**
   * \brief Constructor
   * \param client is the version of the client
   * \param server is the version of the server
   */
  VersionManager(const UMS_Data::Version_ptr& client,
                 const UMS_Data::Version_ptr& server);
  /**
   * \brief Destructor
   * \fn ~VersionManager()
   */
  ~VersionManager();
  /**
  * \brief To check the compatibility between the client and the server
  * \return true if client and server are compatible else false
  */
  bool
  isCompatible();

  /**
  * \brief To check the compatibility between the client and the server
  * \return the error message registered by the version manager
  */
  std::string
  getError() const;

  private:
    /**
     * \brief The object which encapsulates the client version
     */
    UMS_Data::Version_ptr mclient;

    /**
     * \brief The object which encapsulates the server version
     */
    UMS_Data::Version_ptr mserver;
    /**
     * \brief The error message registered by the version manager
     */
    std::string merrormsg;
};


#endif // _VERSION_MANAGER_H_
