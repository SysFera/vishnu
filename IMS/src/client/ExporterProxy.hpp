/**
  * \file ExporterProxy.hpp
  * \brief This file contains the VISHNU ExporterProxy class.
  * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
  * \date March 2011
  */

#ifndef _EXPORTER_PROXY_H
#define _EXPORTER_PROXY_H

#include "IMS_Data.hpp"
#include "SessionProxy.hpp"
#include "MachineProxy.hpp"

/**
 * \class ExporterProxy
 * \brief ExporterProxy class implementation
 */
class ExporterProxy {

  public:
  /**
  * \param session The object which encapsulates the session information
  * \brief Constructor
  */
  explicit ExporterProxy(const SessionProxy& session);

 /**
  * \brief Function to export all the commands launched by a user during a session
  * \param oldSessionId The identifier of a session closed to export
  * \param filename The path of the output file containing the Vishnu shell commands
  * \param options The data structure which encapsulates the o object export
  * \return raises an exception on error
  */
  int
  exportCmd(const std::string& oldSessionId,
            const std::string& filename,
            IMS_Data::ExportOp options);

  /**
    * \brief Destructor, raises an exception on error
    */
  ~ExporterProxy();

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;

};
#endif //_EXPORTER_PROXY_H
