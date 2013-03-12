/**
 * \file MapperRegistry.hpp
 * \brief This file defines the registry module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */


#ifndef __MAPPERREGISTRY__HH__
#define __MAPPERREGISTRY__HH__

#include <map>
#include <string>

#include "SystemException.hpp"
#include "Mapper.hpp"


class Mapper;
/**
 * \class MapperRegistry
 * \brief Registry of all the mapper
 */
class MapperRegistry{
public :
  /**
   * \brief To add a mapper in the list of active mapper
   * \param s Mapper name
   * \param m The mapper
   * \return 0 on success, an error code otherwise
   */
  int
  addMapper(const std::string& s, Mapper* m);
  /**
   * \brief To get a mapper
   * \param name Name of the mapper needed (IN)
   * \return The mapper
   */
  Mapper*
  getMapper(const std::string& name);
  /**
   * \brief To remove a mapper off the list of active mapper
   * \param s Mapper name
   * \return 0 on success, an error code otherwise
   */
  int
  removeMapper(const std::string& s);
  /**
   * \brief Default destructor
   */
  ~MapperRegistry();
  /**
   * \brief To get a mapper registry
   */
  static MapperRegistry*
  getInstance();
protected:
private :
  /**
   * \brief Default constructor
   */
  MapperRegistry();
  /**
   * \brief Exception to be thrown
   */
  SystemException me;
  /**
   * \brief Return if the list contains the string
   * \param s The searched string
   * \return Return true if the list contains the s string
   */
  bool
  contains(const std::string& s);
  /**
   * \brief The registry
   */
  static MapperRegistry* mreg;
  /**
   * \brief Map of the map
   */
  std::map<std::string, Mapper*> mmapper;
  // DEBUG
  /**
   * \brief Debug function
   */
  void
  display();
};

#endif // MAPPERREGISTRY
