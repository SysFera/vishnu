/**
 * \file emfUtils.hpp
 * \brief This file implements the utils functions for EMF object on the module TMS
 * \author Daouda TRAORE (daouda.traore@sysfera.com) et Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 06/04/2011
 */

#ifndef _EMFUTILS_H_
#define _EMFUTILS_H_

#include "ecore.hpp" // Ecore metamodel
#include "ecorecpp.hpp" // EMF4CPP utils
#include "TMS_Data.hpp"

namespace vishnu {

  /**
   * \brief Function to parse the EMF object
   * \param objectSerialized the EMF object serialized
   * \param object_ptr the object build with the objectSerialized
   * \param msgComp an exception message
   * \return  true if success else false
   */
  template<class T>
  bool parseTMSEmfObject(const std::string& objectSerialized, T*& object_ptr, const std::string msgComp=std::string()) {

    object_ptr = NULL;
    try {
      //CREATE DATA MODEL
      TMS_Data::TMS_DataPackage_ptr ecorePackage = TMS_Data::TMS_DataPackage::_instance();
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
#endif //_EMFUTILS_H_
