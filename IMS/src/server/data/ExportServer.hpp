/**
 * \file ExportServer.hpp
 * \brief This file defines the processes interaction with the database
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __EXPORTSERVER__HH__
#define __EXPORTSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"

using namespace std;

class ExportServer{
public:
  /**
   * \brief Constructor
   * \param sessionKey: The session key of the session to export
   */
  ExportServer(string sessionKey);
  /**
   * \brief Destructor
   */
  ~ExportServer();
  /**
   * \brief To export the commands made in the oldSession in the file filename with the options op
   * \param oldSession: Session id of the old session to export
   * \param filename: The name of the file to export the commands
   * \param op: Options for the export
   */
  virtual int 
  exporte(string oldSession, string filename, IMS_Data::ExportOp op) = 0;
protected:
  string mkey;
private:
};


#endif
