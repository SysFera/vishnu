/**
 * \file utilVishnu.cpp
 * \brief This file implements the utils functions of the vishnu system
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/02/2011
 */

#ifndef _UTILSERVER_H_
#define _UTILSERVER_H_

#include "ecore.hpp" // Ecore metamodel
#include "ecorecpp.hpp" // EMF4CPP utils
#include "UMS_Data.hpp"
//#include "TMS_Data.hpp"

namespace vishnu {

  /**
   * \brief Function to get information from the table vishnu
   * \param attrname the name of the attribut
   * \param vishnuid the id of the vishnu configuration
   * \return the corresponding attribut
   */
  std::string
  getAttrVishnu(std::string attrname, std::string vishnuid);

  /**
   * \brief Function to increment a counter of the table vishnu
   * \fn int  incrementCpt(std::string cptName, int cpt)
   * \param cptName the name of the counter to increment
   * \param cpt     the current value of the counter
   * \return raises an exception
   */
  void
  incrementCpt(std::string cptName, int cpt);


  /**
   * \brief Function to parse the EMF object
   * \param objectSerialized the EMF object serialized
   * \param object_ptr the object build with the objectSerialized
   * \param msgComp an exception message
   * \return  true if success else false
   */
  template<class T>
  bool parseEmfObject(const std::string& objectSerialized, T*& object_ptr, const std::string msgComp=std::string()) {

    object_ptr = NULL;
    try {
      //CREATE DATA MODEL
      UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
      ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

      //Parse the model
      ecorecpp::parser::parser parser;
      object_ptr = parser.load(objectSerialized)->as< T >();
    }
    catch (std::exception& e) {
      return false;
    }
    return true;
  }
}

#endif //_UTILSERVER_H_
