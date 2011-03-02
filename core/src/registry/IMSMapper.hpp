/**
 * \file IMSMapper.hpp
 * \brief Mapper for the IMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __IMSMAPPER__HH__
#define __IMSMAPPER__HH__

#include <map>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"


using namespace std;

/**
 * \class IMSMapper
 * \brief Mapper implementation for the IMS module
 */
class IMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \fn IMSMapper(MapperRegistry* reg, string name)
   * \param reg Registry to register
   * \param name Name to use to register
   */
  IMSMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   * \fn IMSMapper()
   */
  IMSMapper();
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper();

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int 
  unregisterMapper();

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand(const int& key,string& command)
   * \param key The key to use
   * \param command OUT, the command corresponding to the key
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const int& key,string& command);

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey(const string& command, int& key)
   * \param command The 'key' to use
   * \param key OUT, the key corresponding to the command
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, int& key);

protected:
private:
};


#endif
