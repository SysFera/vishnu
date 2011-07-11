/**
 * \file TMSShellMapper.hpp
 * \brief Mapper for the TMS module in shell format
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 23/05/11
 */

#ifndef __TMSSHELLMAPPER__HH__
#define __TMSSHELLMAPPER__HH__


#include "TMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"

using namespace std;



/**
 * \class TMSShellMapper
 * \brief Mapper implementation for the TMS module
 */
class TMSShellMapper : public TMSMapper{
public :
  /**
   * \brief Constructor
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  TMSShellMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   */
  TMSShellMapper();
  /**
   * \brief Destructor
   */
  ~TMSShellMapper();

protected:

private:
};


#endif
