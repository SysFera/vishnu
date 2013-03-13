/**
 * \file Mapper.hpp
 * \brief This file defines the generic mapper class
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */

#ifndef __MAPPER__HH__
#define __MAPPER__HH__

#include <map>
#include <string>
#include <vector>

#include <pthread.h>

class MapperRegistry;

/**
 * \class Mapper
 * \brief Generic mapper
 */
class Mapper{
public :
  /**
   * \brief Constructor
   * \fn Mapper(MapperRegistry* reg)
   * \param reg Registry to register
   */
  Mapper(MapperRegistry* reg);
  /**
   * \brief Default constructor
   * \fn Mapper()
   */
  Mapper();
  /**
   * \brief Destructor
   * \fn ~Mapper()
   */
  ~Mapper();
  /**
   * \brief Copy constructor
   * \fn Mapper(const Mapper& m)
   * \param m The mapper to copy
   */
  Mapper(const Mapper& m);
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper(){return 0;}

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  unregisterMapper() {return 0;}

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand(const std::string& key,std::string& command)
   * \param key The key to use
   * \param command OUT, the command corresponding to the key
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const std::string& key, std::string& command) {return 0;}

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey(const std::string& command, std::string& key)
   * \param command The 'key' to use
   * \param key OUT, the key corresponding to the command
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const std::string& command, std::string& key) {return 0;}

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const string& cmd, unsigned int code = 0)
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const std::string& cmd, unsigned int code = 0) {return 0;}

  /**
   * \brief To end a command, delete it from the map and get its value
   * \fn virtual std::string finalize(int key)
   * \param key The entry in the map to get an remove
   * \return The coded value of the command
   */
  virtual std::string
  finalize(int key);

  /**
   * \brief To get, from a coded string, the CPP command that made it
   * \fn virtual std::string decode (const std::string& msg)
   * \param msg The coded string
   * \return The CPP command
   */
  virtual std::string
  decode(const std::string& msg) {return "";}

protected:
  /**
   * \brief To find all the separator in the string s and fill the vector with their position in the string
   * \fn void findSeparator(const std::string& s, std::vector<int>& vec)
   * \param s The string to find the separator
   * \param vec A vector containing the position of the separator in the string s (OUT)
   * \return The cli like close command
   */
  void
  findSeparator(const std::string& s, std::vector<unsigned int>& vec);

  /**
   * \brief The mapper registry
   */
  MapperRegistry* mreg;
  /**
   * \brief The map containing the function and their code
   */
  std::map<int, std::string> mmap;
  /**
   * \brief The name of the mapper
   */
  std::string mname;

  /**
   * \brief Map used to store the string that are decoded
   */
  std::map<int, std::string> mcmd;
  /**
   * \brief Mutex to protect the id generation for the mcmd map
   */
  pthread_mutex_t mutex;
private:
};


#endif
