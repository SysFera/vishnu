/**
 * \file LSFParser.hpp
 * \brief This file presents the implementation of the LSF Parser
 * \author daouda.traore (daouda.traore@sysfera.com)
 * \date April
*/

#ifndef _LSFPARSER_H_
#define _LSFPARSER_H_


/**
 * \class LSFParser
 * \brief This class describes the TMS Parser
 */
class LSFParser {

public :

   /**
    * \brief Constructor
    */
   LSFParser();

  /**
   * \brief Destructor
   */
  ~LSFParser();

   /**
   * \brief To parse a given file
   * \return raises an exception
   */
  static int
  parse_file(const char* pathTofile, struct submit* req);

private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

};
#endif // LSFPARSER
