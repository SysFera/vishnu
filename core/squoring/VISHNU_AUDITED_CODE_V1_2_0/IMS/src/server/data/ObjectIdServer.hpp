/**
 * \file ObjectIdServer.hpp
 * \brief This file defines the object id format server
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 19/04/11
 */

#ifndef __OBJECTIDSERVER__HH__
#define __OBJECTIDSERVER__HH__

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "SessionServer.hpp"
#include "SOCIDatabase.hpp"

using namespace std;

/**
 * \class ObjectIdServer
 * \brief Class that implements the object id format server
 */
class ObjectIdServer{
public:
  /**
   * \brief Constructor
   */
  ObjectIdServer(const UserServer session);
  /**
   * \brief Destructor
   */
  ~ObjectIdServer();
  /**
   * \brief To set the format of a user
   */
  void
  setUID(string fmt);
  /**
   * \brief To set the format of a file transfer
   */
  void
  setFID(string fmt);
  /**
   * \brief To set the format of a task
   */
  void
  setTID(string fmt);
  /**
   * \brief To set the format of a machine
   */
  void
  setMID(string fmt);
protected:
private:
  /**
   * \brief Test if fmt contains '$CPT' as a substring
   * \param fmt: The format string to test
   * \return true if the string fmt contains the '$cpt' substring
   */
  bool
  containCpt(string fmt);
  /**
   * \brief The id of vishnu
   */
  int mvishnuId;
  /**
  * \brief An instance of vishnu database
  */
  SOCIDatabase *mdatabase;
  /**
   * \brief The session
   */
  UserServer msession;
};

#endif // OBJECTIDSERVER
