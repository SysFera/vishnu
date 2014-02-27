/**
 * \file ShellExporter.hpp
 * \brief This file defines the processes interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __SHELLEXPORTER__HH__
#define __SHELLEXPORTER__HH__

#include <string>
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "ExportServer.hpp"

static const std::string UMSSHELLMAPPERNAME = "UMSSHELL";
static const std::string TMSSHELLMAPPERNAME = "TMSSHELL";
static const std::string FMSSHELLMAPPERNAME = "FMSSHELL";


/**
 * \class ShellExporter
 * \brief Class to export in shell format
 */
class ShellExporter: public ExportServer{
public:
  /**
   * \brief Constructor
   * \param u: The user server
   */
  ShellExporter(UserServer u);
  /**
   * \brief Destructor
   */
  ~ShellExporter();
  /**
   * \brief To export the commands made in the oldSession in the file filename with the options op in the shell format
   * \param oldSession: Session id of the old session to export
   * \param content: The content of the export (OUT)
   * \return Succes, an error code otherwise
   */
  int
  exporte(std::string oldSession, std::string &content);
protected:
private:
  /**
   * \brief Return true if the session with the session sid sid is closed
   * \param sid: A session id
   * \return False is the session does not exist or is open
   */
  bool
  isClosed(std::string sid);
  /**
   * \brief Check if the user is allowed to get the list of commands
   */
  bool
  isAllowed(std::string oldSession, UserServer muser);
  /**
   * \brief To get the name of the mapper for the shell
   */
  std::string
  getMapperName(int type);

};


#endif
