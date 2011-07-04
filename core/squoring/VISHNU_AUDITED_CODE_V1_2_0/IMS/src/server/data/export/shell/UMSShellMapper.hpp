/**
 * \file UMSShellMapper.hpp
 * \brief Mapper for the UMS module in shell format
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 23/05/11
 */

#ifndef __UMSSHELLMAPPER__HH__
#define __UMSSHELLMAPPER__HH__


#include "UMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"

using namespace std;



/**
 * \class UMSShellMapper
 * \brief Mapper implementation for the UMS module
 */
class UMSShellMapper : public UMSMapper{
public :
  /**
   * \brief Constructor
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  UMSShellMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   */
  UMSShellMapper();
  /**
   * \brief Destructor
   */
  ~UMSShellMapper();

private:
};


#endif
