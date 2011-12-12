/**
 * \file NetrcReader.hpp
 * \brief This file contains the netrc file reader class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * and Eugene PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date December 2011
 */

#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sys/stat.h>
#include "NetrcReader.hpp"
#include "UserException.hpp"

/**
  * \fn NetrcReader(std::string path)
  * \brief Constructor
  */
NetrcReader::NetrcReader() {
  mpath = boost::filesystem::path(boost::filesystem::path(getenv("HOME"))
                                  /
                                  boost::filesystem::path(".netrc")).string();
}


/**
  * \fn NetrcReader(std::string path)
  * \param path  The path until the netrc file
  * \brief Constructor
  */
NetrcReader::NetrcReader(std::string path) {
  mpath = path;
}

/**
  * \brief Function to read the netrc file for getting the login and the password for
  * a specific machine
  * \param login the login returned which is associated to the machine
  * \param pwd the password returned which is associated to the machine
  * \param machineName the name of the machine defined on the netrc file
  * \return the corresponding couple (login, password)
  */
void
NetrcReader::read(string& login, string& pwd, const string& machineName) {
  NetrcReader netrcReader;
  netrcReader.check();

  pair<string, string> resPair = netrcReader.getNetrcInfo(machineName);
  login = resPair.first;
  pwd = resPair.second;
}


/**
 * \fn ~NetrcReader
 * \brief Destructor
 */
NetrcReader::~NetrcReader() {
}

/**
  * \brief Function to get value associated to a key
  * \param key the key associated to a value on the file
  * \param line the line where the value of the key is searched
  * \param tab a table which allows to register the couple key value
  * \param machine_pos
  * \return true if the value has found else found
  */
bool
NetrcReader::searchValueFromKey(const string& key,
                                string& value,
                                vector<string>& tokens,
                                std::map<size_t, pair<string,string> >& tab,
                                const size_t machine_pos) {

  vector<string>::iterator beg = tokens.begin();
  vector<string>::iterator end = tokens.end();
  size_t size = end-beg;

  if(machine_pos < size ) {
    vector<string>::iterator iter = find(beg+machine_pos, end, key);
    value = "";

    if (iter!=end) {
      vector<string>::iterator keyIter = iter;
      size_t keyPos = keyIter-beg;
      iter++;
      if(iter!=end) {
        value = *iter;
      }
      tab[keyPos] = make_pair<string,string>(key, value);
      return true;
    }
  }
  return false;
}

/**
  * \brief Function to get the login and the password associated to a specific machine from a map
  * \param tab a table in which the couple key and value are registered
  * \param machineName the machine name used for searching the couple
  * \return the login and password associated to the corresponding machineName
  */
pair<string,string>
NetrcReader::getIdentifiers(std::map<size_t, pair<string,string> >& tab, const string& machineName) {
  std::map<size_t, pair<string,string> >::iterator iter;
  std::map<size_t, pair<string,string> >::iterator end = tab.end();
  for(iter = tab.begin(); iter!=tab.end(); iter++) {
    if(iter->second == pair<string,string> ("machine", machineName)) {
      break;
    }
  }
  //if the machine name is not found
  if(iter == end) {
     throw UserException(ERRCODE_INVALID_PARAM, "The machine " + machineName + " is undefined");
  }

  iter++; //to go to the next element
  if (iter == end  || (iter->second).first.compare("login") != 0) {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "The login is undefined. The password must follow the login");
  }
  std::string login = (iter->second).second;
  iter++; //go to to the next element
  if (iter == end  || (iter->second).first.compare("password") != 0) {
    throw UserException(ERRCODE_INVALID_PARAM, "The password is undefined");
  }
  std::string password = (iter->second).second;
  //return login and password values
  return pair<string, string>(login, password);
}

/**
  * \brief Function to check the validity of the netrc file
  * \fn  void check()
  * \return raises an exception on error
  */
void
NetrcReader::check() {

  bool ret = true;
  struct stat fileStat;
  boost::filesystem::path file (mpath);

  // Check the existence of file
  if (!boost::filesystem::exists(file)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The file " + mpath + " does not exist");
  }

  // Check the regularity of the file
  if (!boost::filesystem::is_regular_file(file)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The file " + mpath + " is not a regular file");
  }

  //check if the file is empty
  if (boost::filesystem::is_empty(file)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The file " + mpath + " is empty");
  }

  if (stat(mpath.c_str(), &fileStat)) {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "There is problems to get the permissions of the file: " + mpath);
  }

  std::ostringstream out;
  out << oct << fileStat.st_mode;

  if (out.str().compare("100600") != 0) {
      throw UserException(ERRCODE_INVALID_PARAM,
                        "The permissions of the file: " + mpath + " should be 600");
  }
}

/**
  * \brief Function to get the login and the password associated to a specific machine
  * by parsing the netrc file
  * \param machineName the name of the machine
  * \return the corresponding couple (login, password)
  */
pair< string, string >
NetrcReader::getNetrcInfo(const std::string& machineName) {

  ifstream infile;
  string line;
  string machineValue;
  string loginValue;
  string passwordValue;

  infile.open (mpath.c_str());

  std::map<size_t, pair<string,string> > tab;
  std::string fileContent;

  if (infile.is_open()) {
    while (!infile.eof()) {
      getline(infile, line);
      fileContent = fileContent+ " "+line;
    }
    infile.close();

    vector<string> tokens;
    std::istringstream iss(fileContent);
    std::copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter<vector<string> >(tokens));

    bool machineTest = false;
    bool loginTest = false;
    bool passwordTest = false;
    size_t tokensize = tokens.size();
    size_t machinePos = 0;
    vector<string>::iterator iter;
    vector<string>::iterator beg = tokens.begin();
    vector<string>::iterator end = tokens.end();

    while(machinePos < tokensize) {

      machineTest = searchValueFromKey("machine", machineValue, tokens, tab, machinePos);
      iter = find(beg+machinePos, end, "machine");
      if(machineValue.empty()) {
        machinePos = (iter-beg)+1;
      } else {
        machinePos = (iter-beg)+2;
      }

      loginTest = searchValueFromKey("login", loginValue, tokens, tab, machinePos);
      iter = find(beg+machinePos, end, "login");
      size_t tmp = iter-beg;
      if(tmp <= machinePos){
        if(loginValue.empty()) {
          machinePos +=1;
        } else {
          machinePos +=2;
        }
      }

      passwordTest = searchValueFromKey("password", passwordValue, tokens, tab, machinePos);
      if(!machineTest && !loginTest && !passwordTest){
        break;
      }
    }
    return getIdentifiers(tab, machineName);
  }
  else {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "The file: " + mpath + " can not be opened");
  }
}

