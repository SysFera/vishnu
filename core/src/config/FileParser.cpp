/**
 * \file FileParser.cpp
 * \brief Parser for configuration file
 * \author Gael Le Mahec <gael.le.mahec@ens-lyon.fr>
 * \author Haikel Guemar <hguemar@sysfera.com>
 */

#include "FileParser.hpp"

#include <fstream>
#include <boost/algorithm/string/trim.hpp>  // for trim
#include <map>

#include "CommonParser.hpp"
#include "UserException.hpp"  // for ERRCODE_FILENOTFOUND, etc

FileParser::FileParser() {
}

FileParser::FileParser(const std::string& path) {
  parseFile(path);
}

void
FileParser::parseFile(const std::string& path) {
  std::ifstream file(path.c_str());

  unsigned int l = 0;

  if (!file.is_open()) {
    throw UserException(ERRCODE_FILENOTFOUND, path);
  }

  Splitter split('=');

  while (!file.eof()) {
    char buffer[1024];
    std::string line, key, value;
    std::string::size_type pos;

    l++;
    file.getline(buffer, 1024);
    line = buffer;
    /* Remove comments. */
    pos = line.find('#');
    if (std::string::npos != pos) {
      line = line.substr(0, pos);
    }

    /* Remove leading and trailing white spaces.
     * Caution: do not remove all whitespaces on the line, as we want
     * to allow values containing whitespaces.
     * e.g., tcp://192.168.0.1 machine_1
     */
    boost::algorithm::trim(line);


    /* Empty line => continue. */
    if (line.empty()) {
      continue;
    }

    /* Cut the line on '=' character. */
    split.reset(line);

    std::string& token = split();
    key.swap(token);

    if (!split.hasNext()) {
      debug << "Warning : " << key
            << " has no value! (line " << l << ")\n";
    }

    token = split();
    value.swap(token);

    if (split.hasNext()) {
      debug << "Warning : " << key
            << " has multiple values! "
            << split()
            << " (line " << l << ")\n";
    }


    std::map<std::string, std::string>::iterator it = options_.find(key);
    if (options_.end() == it) {
      options_[key] = value;
    } else {
      debug << "Warning : " << key
            << " already defined! \n"
            << "old value : " << it->second << "\n"
            << "new value : " << value << " (line " << l << ")\n";
    }
  }
}

// use of operator[] is more idiomatic
std::string&
FileParser::operator[](const std::string& key) {
  return options_[key];
}


const std::string&
FileParser::operator[](const std::string& key) const {
  return options_[key];
}


const std::map<std::string, std::string>&
FileParser::getConfiguration() const {
  return options_;
}
