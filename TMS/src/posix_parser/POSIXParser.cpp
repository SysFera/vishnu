/**
 * \file POSIXParser.cpp
 * \brief This file contains the implementation for VISHNU of the posix parser class.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#include "POSIXParser.hpp"

#include <sstream>
#include <fstream>
#include <boost/regex.hpp>


using namespace std;


bool
POSIXParser::parseFile(const string &filePath,
                       map<string, string> &context) {
  ifstream file;
  bool rv = false;
  file.open(filePath.c_str());

  rv = POSIXParser::parseStream(file, context);

  file.close();
  return rv;
}

bool
POSIXParser::parseStream(istream &content,
                         map<string, string> &context) {
  boost::regex reg("^#%\\s*(\\w+)\\s*=\\s*([^\\s]*)(\\s*#.*)?");
  boost::smatch groups;
  string line;
  while (std::getline(content, line)) {
    if (boost::regex_match(line, groups, reg)) {
      context[groups[1]] = groups[2];
    }
  };

  return true;
}
