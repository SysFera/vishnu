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
#include <boost/thread/recursive_mutex.hpp>


/**
 * \brief This class represents the annuary to store the services
 * \class Annuary
 */
class Annuary {
public:
  /**
   * \brief Default constructor
   */
  Annuary() {}
  /**
   * \brief Constructor
   * \param serv
   */
  Annuary(const std::vector<boost::shared_ptr<Server> >& serv);
  /**
   * \brief destructor
   */
  ~Annuary() {}

  /**
   * \brief Add a server in the annuary
   * \param name The name of the server (e.g. UMS, TMS, dispatcher)
   * \param uri The uri the server is running
   * \param services The services offered by the server
   * \return 0 on success, an error code otherwise
   */
  int
  add(const std::string& name, const std::string& uri, const std::vector<std::string>& services);

  /**
   * \brief Remove the server called name listening on port at address of the annuary
   * \param name The name of the server (e.g. UMS, TMS, dispatcher)
   * \param uri The uri of the annuary
   * \return 0 on success, an error code otherwise
   */
  int
  remove(const std::string& name, const std::string& uri);

  /**
   * \brief Return all the servers offering the service called service
   * \param service The name of the desired service
   * \return A vector containing all the servers offering service
   */
  std::vector<boost::shared_ptr<Server> >
  get(const std::string& service = "");

  //TODO: clean later
  /**
   * \brief Init the annuary from a file
   * \param file The file to init from
   */
  void
  initFromFile(const std::string& file);


  /**
   * \brief Initialize the annuary with the configuration of the given module
   * \param module The name of the module
   * \param cfgInfo list of the initialisation data
   */
  void
  setInitConfig(const std::string& module, std::vector<std::string>& cfgInfo);

  /**
   * \brief Prints the content of the annuary
   */
  void
  print();

private :
  /**
   * \brief Fill the services for a given server name. Function used to easily create services from given names
   * \param services OUT, the list of services for name
   * \param name The name of the server
   * \param mid The vishnu machine id
   */
  void
  fillServices(std::vector<std::string> & services, const std::string& name,
               const std::string& mid);

  /**
   * \brief The servers
   */
  std::vector<boost::shared_ptr<Server> > mservers;

  /**
   * \brief mutex to lock the annuary
   */
  mutable boost::recursive_mutex mmutex;
};

#endif // __ANNUARY__H__
