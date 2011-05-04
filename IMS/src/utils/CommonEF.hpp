/**
 * \file CommonEF.hpp
 * \brief This file contains the class for the common EF
 * WARNING, the content of this class should not be used by a dev unless he is aware of the risks, it may have a big impact because of the EF strenght
 */

#ifndef __EF__HH__
#define __EF__HH__

//IMS include
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"

class CommonEF {
public:
  /**
   * \brief COnstructor
   */
  CommonEF();
  /**
   * \brief Destructor
   */
  ~CommonEF();
  /**
   * \brief To get a R.EF from the map
   */
  IMS_Data::ListProcesses
  getREF();
protected:
private:
  /**
   * \brief To init the map
   */
  void initMEF();
  /**
   * \brief Map containing the EF
   */
  map<int, IMS_Data::Process> mef;
  
};


#endif // __EF__HH__
