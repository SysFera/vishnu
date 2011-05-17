#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "SessionProxy.hpp"

#include "FileProxy.hpp"

using namespace std;

void FileProxy::setPath(const string& path) {
  this->path = path;
}

void FileProxy::setHost(const string& host) {
  this->host = host;
}

void FileProxy::setOwner(const string& owner) const {
  this->owner = owner;
}

void FileProxy::setGroup(const string& group) const {
  this->group = group;
}

void FileProxy::setPerms(const mode_t perms) const {
  this->perms = perms;
}

void FileProxy::setUid(const uid_t uid) const {
  this->uid = uid;
}

void FileProxy::setGid(const gid_t gid) const {
  this->gid = gid;
}

void FileProxy::setSize(const file_size_t size) const {
  this->size = size;
}

void FileProxy::setAtime(const time_t atime) const {
  this->atime = atime;
}

void FileProxy::setMtime(const time_t mtime) const {
  this->mtime = mtime;
}

void FileProxy::setCtime(const time_t ctime) const {
  this->ctime = ctime;
}

void FileProxy::setType(const file_type_t type) const {
  this->type = type;
}

void FileProxy::exists(const bool exist) const {
  this->exist = exist;
}

FileProxy::FileProxy() {
  srand(time(NULL));
}

FileProxy::FileProxy(const SessionProxy& sessionProxy,const string& path) {
 
  msessionProxy=sessionProxy;
  
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

FileProxy::FileProxy(const FileProxy& file) {
  srand(time(NULL));
  operator=(file);
}

const SessionProxy& FileProxy::getSession() const {
  return msessionProxy;
}

const string& FileProxy::getPath() const {
  return path;
}

const string& FileProxy::getOwner() const {
  if (!isUpToDate()) getInfos();
  return owner;
}

const string& FileProxy::getGroup() const {
  if (!isUpToDate()) getInfos();
  return group;
}

const string& FileProxy::getHost() const {
  return host;
}

mode_t FileProxy::getPerms() const {
  if (!isUpToDate()) getInfos();
  return perms;
}

uid_t FileProxy::getUid() const {
  if (!isUpToDate()) getInfos();
  return uid;
}

gid_t FileProxy::getGid() const {
  if (!isUpToDate()) getInfos();
  return gid;
}

file_size_t FileProxy::getSize() const {
  if (!isUpToDate()) getInfos();
  return size;
}

time_t FileProxy::getAtime() const {
  if (!isUpToDate()) getInfos();
  return atime;
}

time_t FileProxy::getMtime() const {
  if (!isUpToDate()) getInfos();
  return mtime;
}

time_t FileProxy::getCtime() const {
  if (!isUpToDate()) getInfos();
  return ctime;
}

file_type_t FileProxy::getType() const {
  if (!isUpToDate()) getInfos();
  return type;
}

file_host_t FileProxy::getHostType() const {
  if (getHost()=="localhost") return local;
  return remote;
}

bool FileProxy::exists() const {
  if (!isUpToDate()) getInfos();
  return exist;
}

FileProxy& FileProxy::operator=(const FileProxy& file) {
  setPath(file.getPath());
  setOwner(file.getOwner());
  setGroup(file.getGroup());
  setHost(file.getHost());
  setPerms(file.getPerms());
  setUid(file.getUid());
  setGid(file.getGid());
  setSize(file.getSize());
  setAtime(file.getAtime());
  setMtime(file.getMtime());
  setCtime(file.getCtime());
  setType(file.getType());
  exists(file.exists());
  return *this;
}

bool FileProxy::operator==(const FileProxy& file) {
  return getPath()==file.getPath() && getHost()==file.getHost();
}

bool FileProxy::operator<(const FileProxy& file) {
  return getPath()<file.getPath() ||
         (getPath()==file.getPath() && getHost()<file.getHost());
}

string FileProxy::extHost(const string& path) {
  size_t pos;
  
  pos = path.find_first_of(':');
  if (pos == string::npos) return "localhost";
  return path.substr(0, pos);
}

string FileProxy::extName(const string& path) {
  size_t pos;
  
  pos = path.find_first_of(':');
  if (pos == string::npos) return path;
  return path.substr(pos+1);
}

FileProxy::~FileProxy() {}

string basename(const string& path) {
  size_t pos = path.find_last_of('/');
  if (pos==string::npos) return path;
  return path.substr(pos+1);
}

string gen_uuid() {
  ostringstream result;
  
  result << hex << rand()%256 << rand()%256 << rand()%256 << rand()%256;
  result << "-" << rand()%256 << rand()%256;
  result << "-" << rand()%256 << rand()%256;
  result << "-" << rand()%256 << rand()%256 << rand()%256 << rand()%256;
  result << rand()%256 << rand()%256;
  
  return result.str();
}

int endianness() {
#ifdef __BIG_ENDIAN__
  return BG_ENDIAN;
#else
  return LT_ENDIAN;
#endif
}
