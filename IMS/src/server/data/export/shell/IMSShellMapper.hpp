/**
 * \file IMSShellMapper.hpp
 * \brief Mapper for the IMS module in shell format
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 23/05/11
 */

#ifndef __IMSSHELLMAPPER__HH__
#define __IMSSHELLMAPPER__HH__


#include "IMSMapper.hpp"

#include <string>

#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"



/**
 * \class IMSShellMapper
 * \brief Mapper implementation for the IMS module
 */
class IMSShellMapper : public IMSMapper{
public :
  /**
   * \brief Constructor
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  IMSShellMapper(MapperRegistry* reg, std::string name);
  /**
   * \brief Default constructor
   */
  IMSShellMapper();
  /**
   * \brief Destructor
   */
  ~IMSShellMapper();

protected:

private:
};


#endif
