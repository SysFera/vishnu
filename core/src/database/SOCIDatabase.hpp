/**
 * \file SOCIDatabase.hpp
 * \brief This file presents a SQL database.
 * \author Jean-Baptiste Ghoul (jean-baptiste.ghoul@sysfera.com)
 * \date 14/06/12
 */

#ifndef _SOCIDATABASE_H_
#define _SOCIDATABASE_H_

#include "Database.hpp"
#include "SystemException.hpp"
#include <soci/soci.h>
#include <soci/soci-backend.h>
#include <vector>


/*
 * \brief split a string into a vector of string
 * \param s the string to split
 * \param delim the delimiter character delim
 */
std::vector<std::string> split(const std::string &s, char delim);
/*
 * \brief convert standard time to string YYYY-MM-DD HH-MM-SS
 * \note : maybe put it in utilVishnu
 */
std::string convertTmToString(std::tm time);

/**
 * \class SOCIDatabase
 * \brief SOCI implementation of the Database
 */
class SOCIDatabase : public Database{
public :
  /**
   * \brief Function to process the request in the database
   * \param request The request to process
   * \remark the request can be a multiple SQL statements separated by semicolons
   * \remark the last semicolon is optional
   * \param transacId the id of the transaction if one is used
   * \return 0 on success, an error code otherwise
   * \brief NB : requests like create, drop and alter table put the transcation into auto-commit mode
   */
  int
  process(std::string request, int transacId = -1);
  /**
  * \brief To make a connection to the database
  * \fn int connect()
  * \return raises an exception on error
  */
  int
  connect();
  /**
   * \brief To disconnect from the database
   * \fn disconnect()
   * \return 0 on success, an error code otherwise
   */
  int
  disconnect();
  /**
   * \brief Constructor, raises an exception on error
   * \fn SOCIDatabase(DbConfiguration dbConfig)
   * \param dbConfig  the configuration of the database client
   */

  SOCIDatabase(DbConfiguration dbConfig);
  /**
   * \fn ~SOCIDatabase()
   * \brief Destructor, raises an exception on error
   */
  ~SOCIDatabase();
  /**
  * \brief To get the result of a select request
  * \param request The request to process
  * \param transacId the id of the transaction if one is used
  * \return An object which encapsulates the database results
  */
  DatabaseResult*
  getResult(std::string request, int transacId = -1);
  /**
   * \brief To get the type of database
   * \return An enum identifying the type of database
   */
  DbConfiguration::db_type_t
  getDbType() { return mdbtype; };
/**
 * \brief Start a transaction
 * \return The transaction ID
 */
  virtual int
  startTransaction();
/**
 * \brief End a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  endTransaction(int transactionID);
/**
 * \brief Cancel a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  cancelTransaction(int transactionID);
/**
 * \brief To commit a transaction
 * \param transactionID: The ID of the transaction
 */
  virtual void
  flush(int transactionID);
/**
 * \brief To get a unique id
 * \param table: The table to use to generate the id
 * \param fields: The fields of the table
 * \param val: The values of the fields to insert
 * \param tid: The transaction id
 * \return A new integer never returned by this function
 */
  virtual int
  generateId(std::string table, std::string fields, std::string val, int tid);

  /*
   * \brief template function getResult
   *
   */
  template<class I, class O>
  void getResult(std::string request, const I input, O & output)
  {
  	soci::session * conn;
  	int reqPos=-1;
  	conn=getConnection(reqPos);
  	std::vector<std::vector<std::string> > results;
  	std::vector<std::string> attributeNames;


  	try
  	{
  			((conn->prepare)<<request,soci::use(input),soci::into(output));
  	}
  	catch(std::exception const & e)
  	{
  		throw SystemException(ERRCODE_DBERR,std::string("Cannot get query result : \n")+e.what());
  	}

  	releaseConnection(reqPos);
  	return;
  }

private :
  /**
   * \brief To get a valid connexion
   * \fn session & getConnection(int& pos)
   * \param pos The position of the connection gotten in the pool
   * \return A valid and free connection
   */
  soci::session * getConnection(int& pos);

  /**
   * \brief To release a connexion
   * \fn void releaseConnection(int pos)
   * \param pos The position of the connection to release
   */
  void releaseConnection(int pos);


  /////////////////////////////////
  // Attributes
  /////////////////////////////////
  /**
   * \brief The configuration of the database client
   */
  DbConfiguration mconfig;
  /**
   * \brief The pool of connections
   */
  soci::connection_pool* mpool;
  /**
   * \brief The configuration of the database client
   */
  DbConfiguration::db_type_t mdbtype;
  /*
   * \brief pointer to the backend factory
   */
  const struct soci::backend_factory * mbackend;
  /*
   * \brief connection status
   */
  bool is_connected;


  /////////////////////////////////
  // Functions
  /////////////////////////////////


  /*
   * \brief convert a SOCI row into a vector of string
   * \param the row to convert
   */
  std::vector<std::string>
  rowToString(const soci::row & r);
  /*
   * \brief convert a data include in a SOCI row into string
   */
  std::string
  dataToString(const soci::row & r, size_t pos);
  /*
   * \brief convert SOCI rowset of row into a vector of vector of string
   * \param rs : the query result rowset
   * \param attributeNames : the names of the different columns are stocked here
   * \return attributeNames by reference
   */
  std::vector< std::vector<std::string> >
  rowsetToString(soci::rowset<soci::row> rs,std::vector<std::string> & attributeNames);

  /*
   * \brief return the column names of a SOCI row
   */
  std::vector<std::string>
  getRowAttributeNames(const soci::row & rs);

};


#endif // SOCIDATABASE
