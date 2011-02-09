/**
 * \file Mapper.hpp
 * \brief This file defines the generic mapper class
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */

#ifndef __MAPPER__HH__
#define __MAPPER__HH__

#include <map>

#include "MapperRegistry.hpp"
class MapperRegistry;
using namespace std;

/**
 * \class Mapper
 * \brief Generic mapper
 */
class Mapper{
public :
  Mapper(MapperRegistry* reg);
  Mapper();
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper() = 0;

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int 
  unregisterMapper() = 0;

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const string& key,string& command) = 0;

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, string& key) = 0;

protected:
  MapperRegistry* mreg;
private:
};


#endif
