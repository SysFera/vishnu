/**
 * \file UMSMapper.hpp
 * \brief Mapper for the UMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __UMSMAPPER__HH__
#define __UMSMAPPER__HH__

#include <map>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"

using namespace std;

/**
 * \class UMSMapper
 * \brief Mapper implementation for the UMS module
 */
class UMSMapper : Mapper{
public :
  UMSMapper(MapperRegistry* reg, string name);
  UMSMapper();
  ~UMSMapper();
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
   * \fn virtual int getCommand()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const string& key,string& command);

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, string& key);

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const string& cmd, unsigned int code = 0);
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const string& cmd, unsigned int code = 0);

  /**
   * \brief To end a command, delete it from the map and get its value
   * \fn virtual string finalize(int key);
   * \param The entry in the map to get an remove
   * \return The coded value of the command
   */
  virtual string
  finalize(int key);

  /**
   * \brief To get, from a coded string, the CPP command that made it
   * \fn virtual string decodeCPP (const string& msg);
   * \param msg The coded string
   * \return The CPP command
   */
  virtual string
  decodeCPP (const string& msg);


protected:
private:
};


#endif
