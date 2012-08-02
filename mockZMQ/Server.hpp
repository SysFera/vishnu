/**
 * \file Server.hpp
 * \brief This file defines the servers
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __SERVER__H__
#define __SERVER__H__

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class Server {
private:
  /**
   * \brief The name of the server
   */
  std::string mname;
  /**
   * \brief The services offered by the server
   */
  std::vector<std::string> mservices;
  /**
   * \brief The address the server is running over
   */
  std::string maddress;
  /**
   * \brief The port the server is listening to
   */
  int mport;

public:

  /**
   * \brief DEPRECATED deserialize a server from a string
   * \param string The serialized server
   * \return The server object deserialized
   */
  static boost::shared_ptr<Server>
  fromString(std::string);

  /**
   * \brief Constructor
   * \param name The name of the server
   * \param serv The services offered by the server
   * \param addr The address the server is running
   * \param port The port the server is listening to
   */
  Server(std::string name, std::vector<std::string> &serv, std::string addr, int port);

  /**
   * \brief Destructor
   */
  ~Server();

  /**
   * \brief Add a service to the server
   */
  int
  add(std::string service);

  /**
   * \brief Remove a service to the server
   */
  int
  remove(std::string service);

  /**
   * \brief Return true if the server has a service whose name contains the string service
   * \param service The looked up service
   * \return True if the server offers the service
   */
  bool
  hasService(std::string service);

  /**
   * \brief Getter for the name
   * \return The name of the server
   */
  std::string
  getName();

  /**
   * \brief Getter for the port
   * \return The port the server is listening
   */
  int
  getPort();

  /**
   * \brief Getter for the services
   * \return The list of services of the server
   */
  std::vector<std::string>&
  getServices();

  /**
   * \brief Getter for the address
   * \return The address of the server
   */
  std::string
  getAddress();

  /**
   * \brief DEPRECATED Serializer for the server object
   * \return The serialized server
   */
  std::string
  toString();
};

#endif // __SERVER__H__
