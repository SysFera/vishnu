/**
 * \file ObjectIdProxy.hpp
 * \brief This file defines the object id proxy
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __OBJECTIDPROXY__HH__
#define __OBJECTIDPROXY__HH__

#include <string>
#include "SessionProxy.hpp"


/**
 * \class ObjectIdProxy
 * \brief The object id proxy class implementation
 */
class ObjectIdProxy {
public:
  /**
   * \brief Constructor
   * \param session: The session connected
   */
  ObjectIdProxy(const SessionProxy& session);
  /**
   * \brief Destructor
   */
  ~ObjectIdProxy();
  /**
   * \brief To set the format of the user
   */
  void
  setUID(std::string fmt);
  /**
   * \brief To set the format of the machine
   */
  void
  setMID(std::string fmt);
  /**
   * \brief To set the format of the task
   */
  void
  setTID(std::string fmt);
  /**
   * \brief To set the format of the file transfer
   */
  void
  setFID(std::string fmt);
  /**
   * \brief To set the format of the authentication system
   */
  void
  setAID(std::string fmt);
  /**
   * \brief To set the format of the work
   */
  void
  setWID(std::string fmt);
protected :
private :
  /**
   * \brief To factoryze the diet call code
   */
  void
  defineCall(std::string name, std::string fmt) ;
  /**
  * \brief The object to manipulate the session data
  */
  SessionProxy msessionProxy;

};

#endif // endf objectid proxy
