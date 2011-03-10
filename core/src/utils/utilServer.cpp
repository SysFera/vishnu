/**
 * \file utilServer.cpp
 * \brief This file implements the utils functions of the vishnu system
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/02/2011
 */


#include "POSTGREDatabase.hpp"
#include "DbFactory.hpp"
#include "DatabaseResult.hpp"
#include "utilServer.hpp"
#include "utilVishnu.hpp"

/**
 * \brief Function to get information from the table vishnu
 * \fn    string getAttrVishnu(string attrname, string vishnuid)
 * \param attrname the name of the attribut
 * \param vishnuid the id of the vishnu configuration
 * \return the corresponding attribut
 */
std::string
vishnu::getAttrVishnu(std::string attrname, std::string vishnuid) {

  DbFactory factory;
  Database *databaseVishnu;
  DatabaseResult *result;

  std::string sqlCommand("SELECT "+attrname+" FROM vishnu where vishnuid="+vishnuid);

  databaseVishnu = factory.getDatabaseInstance();
  result = databaseVishnu->getResult(sqlCommand.c_str());
  return result->getFirstElement();

}
/**
 * \brief Function to increment a counter of the table vishnu
 * \fn    incrementCpt(std::string cptName, int cpt)
 * \param cptName the name of the counter to increment
 * \param cpt     the current value of the counter
 * \return raises an exception
 */
void
vishnu::incrementCpt(std::string cptName, int cpt) {

  DbFactory factory;
  Database *databaseVishnu;

  cpt = cpt+1;

  std::string sqlCommand("UPDATE vishnu set "+cptName+"="+vishnu::convertToString(cpt));

  databaseVishnu = factory.getDatabaseInstance();
  databaseVishnu->process(sqlCommand.c_str());

}
