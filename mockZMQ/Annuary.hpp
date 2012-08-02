/**
 * \file Annuary.hpp
 * \brief This file defines the Annuary used to keep the servers
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __ANNUARY__H__
#define __ANNUARY__H__

#include "Server.hpp"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

/**
 * \brief This class represents the annuary to store the services
 * \class Annuary
 */
class Annuary {

public :

  /**
   * \brief Default constructor
   */
  Annuary();
  /**
   * \brief Copy constructor
   */
  ~Annuary();

  /**
   * \brief Add a server in the annuary
   * \param name The name of the server (e.g. UMS, TMS, namer)
   * \param services The services offered by the server
   * \param port The port the server is listening
   * \param port The address the server is running
   * \return 0 on success, an error code otherwise
   */
  int
  add(std::string name, std::vector<std::string> services, int port, std::string address);

  /**
   * \brief Remove the server called name listening on port at address of the annuary
   * \param name The name of the server (e.g. UMS, TMS, namer)
   * \param port The port the server is listening
   * \param port The address the server is running
   * \return 0 on success, an error code otherwise
   */
  int
  remove(std::string name, int port, std::string address);

  /**
   * \brief Return all the servers offering the service called service
   * \param service The name of the desired service
   * \return A vector containing all the servers offering service
   */
  std::vector<boost::shared_ptr<Server> >*
  get(std::string service);

  /**
   * \brief Init the annuary from a file
   * \param file The file to init from
   */
  void
  initFromFile(std::string file);


private :
  /**
   * \brief Fill the services for a given server name. Function used to easily create services from given names
   * \param services OUT, the list of services for name
   * \param name The name of the server
   */
  void
  fillServices(std::vector<std::string> & services, std::string name);

  /**
   * \param The servers
   */
  std::vector<boost::shared_ptr<Server> > mservers;
}; // End class annuary

#endif // __ANNUARY__H__
