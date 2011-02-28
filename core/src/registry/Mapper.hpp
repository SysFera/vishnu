/**
 * \file Mapper.hpp
 * \brief This file defines the generic mapper class
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */

#ifndef __MAPPER__HH__
#define __MAPPER__HH__

#include <map>

#include <pthread.h>
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
  ~Mapper();
  Mapper(const Mapper& m);
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper(){return 0;}

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  unregisterMapper() {return 0;}

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const string& key,string& command) {return 0;}

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey() = 0
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, string& key) {return 0;}

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const string& cmd, unsigned int code = 0) = 0;
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const string& cmd, unsigned int code = 0) {return 0;}

  /**
   * \brief To end a command, delete it from the map and get its value
   * \fn virtual string finalize(int key) = 0;
   * \param The entry in the map to get an remove
   * \return The coded value of the command
   */
  virtual string
  finalize(int key) {return "";}

  /**
   * \brief To get, from a coded string, the CPP command that made it
   * \fn virtual string decodeCPP (const string& msg) = 0 ;
   * \param msg The coded string
   * \return The CPP command
   */
  virtual string
  decodeCPP (const string& msg) {return "";}

protected:
  /**
   * \brief The mapper registry
   */
  MapperRegistry* mreg;
  /**
   * \brief The map containing the function and their code
   */
  map<int, string> mmap;
  /**
   * \brief The name of the mapper
   */
  string mname;

  /**
   * \brief Map used to store the string that are decoded
   */
  map<int, string> mcmd;
  /**
   * \brief Mutex to protect the id generation for the mcmd map
   */
  pthread_mutex_t mutex;
private:
};


#endif
