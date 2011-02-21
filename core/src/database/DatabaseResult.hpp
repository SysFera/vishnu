/**
 * \file DatabaseResult.hh
 * \brief This file presents the Database results class.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi.sysfera.com)
 * \date 15/12/10 
 */

#ifndef _DATABASERESULT_H_
#define _DATABASERESULT_H_

#include <string>
#include <vector>

/**
 * \class DatabaseResult
 * \brief This class describes the object which encapsulates the database results
 */
class DatabaseResult{
public :
  /**
   * \brief Function to print the database results
   * \fn    print()
   * \param request The request to process
   * \return 0 on success, an error code otherwise
   */
  void 
  print();
  /**
   * \brief Function to print the attributes names
   * \fn    print()
   * \param request The request to process
   * \return 0 on success, an error code otherwise
   */
  void 
  printAttributesNames();
  /**
   * \brief To get the number of tuples 
   * \fn size_t getNbTuples() 
   * \return 0 on success, an error code otherwise
   */
  size_t 
  getNbTuples() const;
  /**
   * \brief To get the number of fields 
   * \fn getNbFields() 
   * \return 0 on success, an error code otherwise
   */
  size_t 
  getNbFields() const;
  
  /**
   * \brief To get a specific results using its position
   * \fn get(int position); 
   * \param position The position of the request
   * \return the tuple associated to the postion
   */
  std::vector<std::string> 
  get(unsigned int position) const;
  /**
   * \brief To get the number of fields 
   * \fn get(); 
   * \return all results
   */
  std::vector<std::vector<std::string> >
  getResults() const;
  /**
   * \brief To get the first attribut value of the first element 
   * \fn std::string getFirstElement(); 
   * \return the first attribut value of the first element
   */
   std::string
   getFirstElement() const;
  /**
   * \fn DatabaseResult()
   * \brief Constructor, raises an exception on error
   */
  DatabaseResult(std::vector<std::vector<std::string> > res, std::vector<std::string> namesAttributes);
  /**
   * \fn ~DatabaseResult()
   * \brief Destructor, raises an exception on error
   */
 ~DatabaseResult();
private :
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The number of fields
   */
   size_t fields;
  /**
   * \brief The number of tuples
   */
   size_t tuples;
  /**
   * \brief The database results
   */
  std::vector<std::vector<std::string> > results;
  /**
   * \brief The attributes names
   */
  std::vector<std::string> attributesNames;
  /////////////////////////////////
  // Functions
  /////////////////////////////////
  
  /**
   * \brief To set the number of tuples 
   * \fn setNbTuples(size_t nbtuples)
   * \param nbtuples The number of tuples
   * \return 0 on success, an error code otherwise
   */
  void 
  setNbTuples(size_t nbtuples);
  /**
   * \brief To set the number of fields 
   * \fn setNbFields(size_t nbfields)
   * \param nbtuples The number of fields
   * \return 0 on success, an error code otherwise
   */
  void 
  setNbFields(size_t nbfields);
  
};

#endif // DATABASERESULT
