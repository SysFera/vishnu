/**
 * \file utilClient.hpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Benjamin Isnard
 * \date April 2011
 */
#ifndef  _utilClient_H_
#define  _utilClient_H_

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "SystemException.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "IMSVishnuException.hpp"
#include "FMSVishnuException.hpp"
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif

/**
 * \brief Function to spread error message to C++ API, Python API, Web service API and command line program
 * \fn void raiseDietMsgException(const std::string& msg)
 * \param msg to spread
 */
VISHNU_API_LIB void  raiseDietMsgException(const std::string& msg);

/**
 * \brief Function to split the receiving message into error code and message error
 * \fn void raiseExceptionIfNotEmptyMsg(const std::string& msg)
 * \param msg to split
 */
VISHNU_API_LIB void raiseExceptionIfNotEmptyMsg(const std::string& msg);

/**
 * \brief Function to parse the EMF object
 * \param objectSerialized the EMF object serialized
 * \param object_ptr the object build with the objectSerialized
 * \param msgComp an exception message
 * \return raises an exception on error
 */
template<class T>
VISHNU_API_LIB void parseEmfObject(const std::string& objectSerialized, T*& object_ptr, const std::string msgComp=std::string()) {

  object_ptr = NULL;
  try {
    //CREATE DATA MODEL
    T tmpObject;
    ecore::EPackage_ptr ecorePackage = tmpObject._eClass()->getEPackage();
    ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

    //Parse the model
    ecorecpp::parser::parser parser;
    object_ptr = parser.load_str(objectSerialized)->as< T >();
  }
  catch (std::exception& e) {
    throw SystemException(ERRCODE_INVDATA, msgComp);
  }

}
#endif
