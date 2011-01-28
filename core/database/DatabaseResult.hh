/**
 * \file DatabaseResult.hh
 * \brief This file presents an abstract database.
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
  int 
  print();
  /**
   * \brief To get the number of tuples 
   * \fn getNbTuples() 
   * \return 0 on success, an error code otherwise
   */
  int 
  getNbTuples();
  /**
   * \brief To get the number of fields 
   * \fn getNbFields() 
   * \return 0 on success, an error code otherwise
   */
  int 
  getNbFields();
  /**
   * \brief To get the number of fields 
   * \fn get(int position); 
   * \param position The position of the request
   * \return the tuple associated to the postion
   */
  std::vector<std::string> 
  get(int position);
  /**
   * \brief To get the number of fields 
   * \fn get(); 
   * \return all results
   */
  std::vector<std::vector<std::string>>
  getResults();
  
protected :
  /**
   * \fn DatabaseResult()
   * \brief Constructor, raises an exception on error
   */
  Database(std::vector<std::vector<std::string>> res);
  /**
   * \fn ~DatabaseResult()
   * \brief Destructor, raises an exception on error
   */
virtual ~Database();
private :
  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The number of fields
   */
   int fields;
  /**
   * \brief The number of tuples
   */
   int tuples;
  /**
   * \brief The database results
   */
  std::vector<std::vector<std::string> > results;
   
   
};

#endif // DATABASERESULT
