/**
 * \file ShellExporter.hpp
 * \brief This file defines the processes interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __SHELLEXPORTER__HH__
#define __SHELLEXPORTER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"

#include "ExportServer.hpp"

class ShellExporter: public ExportServer{
public:
  /**
   * \brief Constructor
   * \param sessionKey: The session key of the session to export
   */
  ShellExporter(string sessionKey);
  /**
   * \brief Destructor
   */
  ~ShellExporter();
  /**
   * \brief To export the commands made in the oldSession in the file filename with the options op in the shell format
   * \param oldSession: Session id of the old session to export
   * \param filename: The name of the file to export the commands
   * \param op: Options for the export
   * \return Succes, an error code otherwise
   */
  int
  exporte(string oldSession, string filename, IMS_Data::ExportOp op);
protected:
private:
};


#endif

