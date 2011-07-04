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
 * \brief To update the name of the file owner
 * \param owner the new owner
 */  

void FileProxy::setOwner(const string& owner) const {
  mfileStat.setOwner(owner);

}
/**
 * \brief To update the group of the file owner
 * \param group the new group
 */  

void FileProxy::setGroup(const string& group) const {
  mfileStat.setGroup(group);
}
/**
 * \brief To update the access permissions of the file
 * \param perms the new access permissions
 */  

void FileProxy::setPerms(const mode_t perms) const {
  mfileStat.setPerms(perms);
}
/**
 * \brief To update the identifier of the file owner
 * \param uid the new owner identifier
 */  

void FileProxy::setUid(const uid_t uid) const {
  mfileStat.setUid(uid);
}
/**
 * \brief To update the group identifier of the file owner
 * \param gid the new group identifier
 */  

void FileProxy::setGid(const gid_t gid) const {
  mfileStat.setGid(gid);
}
/**
 * \brief To update the size of the file
 * \param size the new size
 */  

void FileProxy::setSize(const file_size_t size) const {
  mfileStat.setSize(size);
}
/**
 * \brief To update the last access time of the file
 * \param atime the new access time
 */  

void FileProxy::setAtime(const time_t atime) const {
  mfileStat.setAtime(atime);
}
/**
 * \brief To update the last modification time of the file
 * \param mtime the new modofocation time
 */  

void FileProxy::setMtime(const time_t mtime) const {
  mfileStat.setMtime(mtime);
}
/**
 * \brief To update the last change time of the file
 * \param ctime the new change time
 */  

void FileProxy::setCtime(const time_t ctime) const {
  mfileStat.setCtime(ctime);
}
/**
 * \brief To update the file type
 * \param type the new  file type
 */  

void FileProxy::setType(const FileType& type) const {
  mfileStat.setType(type);
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
  srand(time(NULL));
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

  srand(time(NULL));
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
  srand(time(NULL));
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
 * \brief To get the name of the file owner
 * \return the file owner name
 */
const string& FileProxy::getOwner() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getOwner();
}

/**
 * \brief To get the group name of the file owner
 * \return the group name
 */
const string& FileProxy::getGroup() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getGroup();
}

/**
 * \brief To get the host in which the file is located
 * \return the host of the file
 */
const string& FileProxy::getHost() const {
  return mhost;
}

/**
 * \brief To get the file access permissions
 * \return the file access permissions
 */
mode_t FileProxy::getPerms() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getPerms();
}

/**
 * \brief To get the identifier of the file owner
 * \return the identifier of the file owner
 */
uid_t FileProxy::getUid() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getUid();
}

/**
 * \brief To get the group identifier of the file owner
 * \return the group identifier of the file owner
 */
gid_t FileProxy::getGid() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getGid();
}

/**
 * \brief To get the size of the file 
 * \return the file size
 */
file_size_t FileProxy::getSize() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getSize();
}

/**
 * \brief To get the last access time of the file
 * \return the last file access time
 */
time_t FileProxy::getAtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getAtime();
}

/**
 * \brief To get the last  modification time of the file
 * \return the last  modification time
 */
time_t FileProxy::getMtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getMtime();
}

/**
 * \brief To get the last change time of the file
 * \return the last change time
 */
time_t FileProxy::getCtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getCtime();
}

/**
 * \brief To get the file type
 * \return the file type
 */
FileType FileProxy::getType() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getType();
}

/**
 * \brief To check if the file exists
 * \return true if the file exists, false otherwise
 */

bool FileProxy::exists() const {
  if (!isUpToDate()) getInfos();
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
  if (pos == string::npos) return "localhost";
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
  if (pos == string::npos) return path;
  return path.substr(pos+1);
}

/**
 * \brief The default destructor
 */
FileProxy::~FileProxy() {}


