/**
 * \file MapperRegistry.hpp
 * \brief This file defines the registry module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */


#ifndef __MAPPERREGISTRY__HH__
#define __MAPPERREGISTRY__HH__

#include <map>

#include "SystemException.hpp"
#include "Mapper.hpp"

using namespace std;
class Mapper;
/**
 * \class MapperRegistry
 * \brief Registry of all the mapper
 */
class MapperRegistry{
public :
  /**
   * \brief To add a mapper in the list of active mapper
   * \fn int addMapper(string s, Mapper* m)
   * \param s Mapper name
   * \param m The mapper
   * \return 0 on success, an error code otherwise
   */
  int
  addMapper(string s, Mapper* m);
  /**
   * \brief To get a mapper
   * \fn Mapper* getMapper(const string& name)
   * \param name Name of the mapper needed (IN)
   * \return The mapper
   */
  Mapper*
  getMapper(const string& name);
  /**
   * \brief To remove a mapper off the list of active mapper
   * \fn int removeMapper(string s)
   * \param s Mapper name
   * \return 0 on success, an error code otherwise
   */
  int
  removeMapper(string s);
  /**
   * \brief Default destructor
   * \fn ~MapperRegistry()
   */
  ~MapperRegistry();
  /**
   * \brief To get a mapper registry
   * \fn static MapperRegistry* getInstance()
   */
  static MapperRegistry*
  getInstance();
protected:
private :
  /**
   * \brief Default constructor
   * \fn MapperRegistry()
   */
  MapperRegistry();
  /**
   * \brief Exception to be thrown
   */
  SystemException me;
  /**
   * \brief Return if the list contains the string
   * \fn bool contains(string s)
   * \param s The searched string
   * \return Return true if the list contains the s string
   */
  bool
  contains(string s);
  /**
   * \brief The registry
   */
  static MapperRegistry* mreg;
  /**
   * \brief Map of the map
   */
  map<string, Mapper*> mmapper;
  // DEBUG
  /**
   * \brief Debug function
   * \fn void display()
   */
  void display();
};

#endif // MAPPERREGISTRY

