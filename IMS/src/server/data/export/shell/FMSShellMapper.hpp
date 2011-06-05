/**
 * \file FMSShellMapper.hpp
 * \brief Mapper for the FMS module in shell format
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 23/05/11
 */

#ifndef __FMSSHELLMAPPER__HH__
#define __FMSSHELLMAPPER__HH__


#include "FMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"

using namespace std;



/**
 * \class FMSShellMapper
 * \brief Mapper implementation for the FMS module
 */
class FMSShellMapper : public FMSMapper{
public :
  /**
   * \brief Constructor
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  FMSShellMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   */
  FMSShellMapper();
  /**
   * \brief Destructor
   */
  ~FMSShellMapper();

protected:

private:
};


#endif
