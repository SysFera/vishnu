#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "File.hpp"

using namespace std;
using namespace FMS_Data;

void File::setPath(const string& path) {
  mfileStat.setPath(path);
}

void File::setHost(const string& host) {
  mhost = host;
}

void File::setOwner(const string& owner) const {
  mfileStat.setOwner(owner);
}

void File::setGroup(const string& group) const {
  mfileStat.setGroup(group);
}

void File::setPerms(const mode_t perms) const {
mfileStat.setPerms(perms);
}

void File::setUid(const uid_t uid) const {
  mfileStat.setUid(uid);
}

void File::setGid(const gid_t gid) const {
  mfileStat.setGid(gid);
}

void File::setSize(const file_size_t size) const {
  mfileStat.setSize(size);
}

void File::setAtime(const time_t atime) const {
  mfileStat.setAtime(atime);
}

void File::setMtime(const time_t mtime) const {
  mfileStat.setMtime(mtime);
}

void File::setCtime(const time_t ctime) const {
  mfileStat.setCtime(ctime);
}

void File::setType(const file_type_t type) const {
  mfileStat.setType(type);
}

void File::exists(const bool exist) const {
  this->exist = exist;
}

File::File():exist("false") {
  
}

File::File(const SessionServer& sessionServer,
           const string& path)
           :msessionServer(sessionServer) {
  
 
  
  size_t pos = path.find(':');
  
  if (pos==string::npos) {
    setPath(path);
    setHost("localhost");
  } else {
    setPath(path.substr(pos+1));
    setHost(path.substr(0,pos));
  }
}

File::File(const File& file) {
  
  operator=(file);
}

const SessionServer& File::getSession() const {
  return msessionServer;
}

const FileStat& File::getFileStat() const{
  return mfileStat;
}

const string& File::getPath() const {
  return mfileStat.getPath();
}

const string& File::getOwner() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getOwner();
}

const string& File::getGroup() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getGroup();
}

const string& File::getHost() const {
  return mhost;
}

mode_t File::getPerms() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getPerms();
}

uid_t File::getUid() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getUid();
}

gid_t File::getGid() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getGid();
}

file_size_t File::getSize() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getSize();
}

time_t File::getAtime() const {
 
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getAtime();
}

time_t File::getMtime() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getMtime();
}

time_t File::getCtime() const {
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getCtime();
}

FileType File::getType() const {
 
  if (!isUpToDate()){
    getInfos();
  }
  return mfileStat.getType();
}

file_host_t File::getHostType() const {
 
  if (getHost()=="localhost") {
    return local;
  }
  return remote;
}

bool File::exists() const {
  if (!isUpToDate()){
    getInfos();
  }
  return exist;
}

File& File::operator=(const File& file) {

  mfileStat= file.getFileStat();
  setHost(file.getHost());
 
  exists(file.exists());
  
  return *this;
}

bool File::operator==(const File& file) {
  return getPath()==file.getPath() && getHost()==file.getHost();
}

bool File::operator<(const File& file) {
  return getPath()<file.getPath() ||
         (getPath()==file.getPath() && getHost()<file.getHost());
}

string File::extHost(const string& path) {
  size_t pos;
  
  pos = path.find_first_of(':');
  if (pos == string::npos) {
    return "localhost";
  }
  return path.substr(0, pos);
}

string File::extName(const string& path) {

  size_t pos;
  
  pos = path.find_first_of(':');
 
  if (pos == string::npos){
    return path;
  }
  return path.substr(pos+1);
}

File::~File() {}


