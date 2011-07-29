/**
 * \file FileProxy.hpp
 * This file declares a  proxy class to handle a file
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \author Ibrahima Cisse (irahima.cisse@sysfera.com)
 */



#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "SessionProxy.hpp"

#include "FileProxy.hpp"
#include "FMS_Data.hpp"

using namespace std;
using namespace FMS_Data;

/**
 * \brief To update the inode information of the file
 * \param fileStat the new inode information
 */ 
void FileProxy::setFileStat(const FileStat& fileStat) const {
  mfileStat=fileStat;
}
/**
 * \brief To update the path  of the file
 * \param path the new path
 */

void FileProxy::setPath(const string& path) {
  mfileStat.setPath(path);
}
/**
 * \brief To update the host  of the file
 * \param host the new host
 */  

void FileProxy::setHost(const string& host) {
  mhost = host;
}

/**
 * \brief To updata the file existence flag
 * \param exist the new file existence flag 
 */  

void FileProxy::exists(const bool exist) const {
  this->exist = exist;
}

/**
 * \brief The default constructor
 */ 
FileProxy::FileProxy() {
  
  this->exist = exist;
}

/**
 * \brief Another constructor taking two parameters
 * \param sessionProxy  the session object proxy
 * \param path the path of the file
 */ 
FileProxy::FileProxy(const SessionProxy& sessionProxy,
    const string& path)
:msessionProxy(sessionProxy)  {

  
  size_t pos = path.find(':');

  if (pos==string::npos) {
    setPath(path);
    setHost("localhost");
  } else {
    setPath(path.substr(pos+1));
    setHost(path.substr(0,pos));
  }
}
/**
 * \brief Another constructor by reference
 * \param file the other file 
 */ 

FileProxy::FileProxy(const FileProxy& file) {
  
  operator=(file);
}

/**
 * \brief To get session proxy object
 * \return the session in which the file was created
 */
const SessionProxy& FileProxy::getSession() const {
  return msessionProxy;
}

/**
 * \brief To get the inode information of the file
 * \return the file inode information
 */
const FileStat& FileProxy::getFileStat() const{
  return mfileStat;
}


/**
 * \brief To get the path of the file
 * \return the file path
 */
const string& FileProxy::getPath() const {
  return mfileStat.getPath();
}


/**
 * \brief To get the host in which the file is located
 * \return the host of the file
 */
const string& FileProxy::getHost() const {
  return mhost;
}


/**
 * \brief To check if the file exists
 * \return true if the file exists, false otherwise
 */

bool FileProxy::exists() const {
  
  if (!isUpToDate()){
    getInfos();
  }
  return exist;
}

/**
 * \brief The assignment operator
 * \param fileProxy the object to assign
 * \return the new assigned file
 */
FileProxy& FileProxy::operator=(const FileProxy& fileProxy) {

  mfileStat= fileProxy.getFileStat();
  setHost(fileProxy.getHost());

  exists(fileProxy.exists());

  return *this;
}

/**
 * \brief a useful function to extract the host name from path
 * \param path the file path 
 * \return the host name
 */

string FileProxy::extHost(const string& path) {
  size_t pos;

  pos = path.find_first_of(':');
  if (pos == string::npos) {
    return "localhost";
  }
  return path.substr(0, pos);
}

/**
 * \brief a useful function to extract the file path name from the complete path
 * \param path the complete file path 
 * \return the file path
 */
string FileProxy::extName(const string& path) {
  size_t pos;

  pos = path.find_first_of(':');
 
  if (pos == string::npos){
    return path;
  }
  return path.substr(pos+1);
}

/**
 * \brief The default destructor
 */
FileProxy::~FileProxy() {}


