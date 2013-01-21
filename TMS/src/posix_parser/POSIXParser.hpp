/**
 * \file POSIXParser.hpp
 * \brief This file contains the VISHNU posix parser class.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#ifndef __POSIXPARSER__HH__
#define __POSIXPARSER__HH__


#include <string>
#include <map>
#include <istream>


/**
 * \class POSIXParser
 * \brief This class provides a parser for retrieving TMS scripts
 *        submission parameters. These parameters are of the form
 *        "#% key = value". The parser also handles comments (#) on a
 *        single ligne, or in a ligne containing key = value
 */
class POSIXParser {
public:
  /**
   * \brief Parses a file
   * \param filePath the command file to parse
   * \param context the context (INOUT) of the job
   * \return true on good parsing
   */
  static bool
  parseFile(const std::string &filePath,
            std::map<std::string, std::string> &context);

  /**
   * \brief Parses a stream
   * \param content the string to parse
   * \param context the context (INOUT) of the job
   * \return true on good parsing
   */
  static bool
  parseStream(std::istream &content,
              std::map<std::string, std::string> &context);
};

#endif
