/**
 * \file QueryProxy.hpp
 * \brief This file contains the VISHNU QueryProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#ifndef _QUERY_PROXY_H_
#define _QUERY_PROXY_H_

#include <string>
#include <iostream>

#include <ecore.hpp>
#include <ecorecpp.hpp>

#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc
#include "DIET_client.h"
#include "utilsClient.hpp"
#include "SessionProxy.hpp"

/**
 * \brief SerializeAdaptor class implementation
 */
template <class Object>
struct SerializeAdaptor {

  /**
   * \brief Function to convert an object type to string
   * \param object The Object to convert
   * \return the serialized object
   */
  static std::string serialize(const Object& object) {
    ::ecorecpp::serializer::serializer _ser;
    return  _ser.serialize_str(const_cast<Object*>(&object));
  }

};

/**
 * \brief Specialization in std::string of the SerializeAdaptor
 */
template <>
struct SerializeAdaptor<std::string> {

  /**
   * \brief Function to convert an object type to string
   * \param object The Object to convert
   * \return the serialized object
   */
  static std::string serialize(const std::string& object) {
    return  object;
  }

};

/**
 * \class QueryProxy
 * \brief QueryProxy class implementation
 */
template <class QueryParameters, class ListObject>
class QueryProxy
{

public:

  /**
   * \param params The object which encapsulates the information of queryProxy options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param serviceName The name of the service to call
   * \brief Constructor, raises an exception on error
   */
  QueryProxy(const QueryParameters& params,
             const SessionProxy& session,
             const std::string& serviceName);
  /**
   * \param params The object which encapsulates the information of queryProxy options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param serviceName The name of the service to call
   * \param machineId The identifier of the machine in which the query will be done
   * \brief Constructor, raises an exception on error
   */
  QueryProxy(const QueryParameters& params,
             const SessionProxy& session,
             const std::string& serviceName,
             const std::string& machineId);

  /**
   * \brief Function to change or reset the machineId
   * \param machineId the id of the machine used by the query
   */
  void
  setMachineId(const std::string& machineId);

  /**
   * \brief Function to get the machineId used by the query
   * \return the id of the machine used by the query
   */
  std::string
  getMachineId() const;

  /**
   * \brief Function to list QueryProxy information
   * \fn  ListObject* list()
   * \return The pointer to the ListOject containing list information
   * \return raises an exception on error
   */
  ListObject*
  list();

  /**
   * \fn ~QueryProxy()
   * \brief Destructor, raises an exception on error
   */
  ~QueryProxy();

private:
  /**
   * \brief Define a pointer type for more facilities
   */
  typedef ListObject* ListObject_ptr;
  /**
   * \brief Define a pointer type for more facilities
   */
  typedef QueryParameters* QueryParameters_ptr;

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
   * \brief The QueryParameters information
   */
  QueryParameters mparameters;
  /**
   * \brief The name of the service
   */
  std::string mserviceName;
  /**
   * \brief The SessionProxy object containing the encrypted identifier of the session
   *  generated by VISHNU
   */
  SessionProxy msessionProxy;
  /**
   * \brief The Object containing query information
   */
  ListObject* mlistObject;
  /**
   * \brief The id of the machine used by the query
   */
  std::string mmachineId;
};

/**
 * \brief A Constructor of the QueryProxy template class
 * \param params The object which encapsulates the information of queryProxy options
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \param serviceName The name of the service to call
 * \brief Constructor, raises an exception on error
 */
template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::QueryProxy(const QueryParameters& params,
                                                    const SessionProxy& session,
                                                    const std::string& serviceName):
  mparameters(params), mserviceName(serviceName),
  msessionProxy(session), mlistObject(NULL) {
}

/**
 * \brief Another constructor of the QueryProxy template class
 * \param params The object which encapsulates the information of queryProxy options
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \param serviceName The name of the service to call
 * \param machineId The identifier of the machine in which the query will be done
 * \brief Constructor, raises an exception on error
 */

template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::QueryProxy(const QueryParameters& params,
                                                    const SessionProxy& session,
                                                    const std::string& serviceName,
                                                    const std::string& machineId):
  mparameters(params), mserviceName(serviceName),
  msessionProxy(session), mmachineId (machineId),
  mlistObject(NULL) {
}


/**
 * \brief Function to reset machineId
 * \param machineId The identifier of the machine in which the query will be done
 */
template <class QueryParameters, class ListObject>
void
QueryProxy<QueryParameters, ListObject>::setMachineId(const std::string& machineId) {
  mmachineId = machineId;
}


/**
 * \brief Function to get the machineId used by the query
 * \return the id of the machine used by the query
 */
template <class QueryParameters, class ListObject>
std::string
QueryProxy<QueryParameters, ListObject>::getMachineId() const {
  return mmachineId;
}

/**
 * \brief Function to list QueryProxy information
 * \fn  ListObject* QueryProxy<QueryParameters, ListObject>::list()
 * \return The pointer to the ListOject containing list information
 * \return raises an exception on error
 */
template <class QueryParameters, class ListObject>
ListObject* QueryProxy<QueryParameters, ListObject>::list() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string queryParmetersToString;
  std::string listObjectInString;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  //If the query uses the machineId (machineId not null)
  if (!mmachineId.empty()) {
    profile = diet_profile_alloc(mserviceName.c_str(), 2, 2, 4);
  } else {
    profile = diet_profile_alloc(mserviceName.c_str(), 1, 1, 3);
  }

  sessionKey = msessionProxy.getSessionKey();
  queryParmetersToString =  SerializeAdaptor<QueryParameters>::serialize(mparameters);
  //IN Parameters
  if (diet_string_set(profile, 0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msg);
  }
  //If the query uses the machineId (machineId not null)
  if (!mmachineId.empty()) {
    if (diet_string_set(profile, 1, mmachineId)) {
      msg += "with machineId parameter "+mmachineId;
      raiseCommunicationMsgException(msg);
    }

    if (diet_string_set(profile, 2, queryParmetersToString)) {
      msg += "with queryParmetersToString parameter "+queryParmetersToString;
      raiseCommunicationMsgException(msg);
    }

    //OUT Parameters
    diet_string_set(profile,3);
    diet_string_set(profile,4);

    if (!diet_call(profile)) {
      if (diet_string_get(profile,3, listObjectInString)){
        msg += "by receiving listObjectInString message";
        raiseCommunicationMsgException(msg);
      }
      if (diet_string_get(profile,4, errorInfo)){
        msg += "by receiving errorInfo message";
        raiseCommunicationMsgException(msg);
      }
    } else {
      raiseCommunicationMsgException("Bus initialization failure");
    }
  } else {
    if (diet_string_set(profile, 1, queryParmetersToString)) {
      msg += "with queryParmetersToString parameter "+queryParmetersToString;
      raiseCommunicationMsgException(msg);
    }

    //OUT Parameters
    diet_string_set(profile,2);
    diet_string_set(profile,3);

    if (!diet_call(profile)) {
      if (diet_string_get(profile,2, listObjectInString)){
        msg += "by receiving listObjectInString message";
        raiseCommunicationMsgException(msg);
      }

      if (diet_string_get(profile,3, errorInfo)){
        msg += "by receiving errorInfo message";
        raiseCommunicationMsgException(msg);
      }
    } else {
      raiseCommunicationMsgException("Bus call failure");
    }
  }

  /* To check the receiving message error */
  raiseExceptionIfNotEmptyMsg(errorInfo);

  //To parse List object serialized
  parseEmfObject(listObjectInString, mlistObject, "Error by receiving List object serialized");

  return mlistObject;
}

/**
 * \brief A default destructor for QueryProxy
 * \brief Destructor, raises an exception on error
 */
template <class QueryParameters, class ListObject>
QueryProxy<QueryParameters, ListObject>::~QueryProxy()
{
}

#endif
