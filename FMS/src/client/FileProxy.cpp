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

void FileProxy::setPath(const string& path) {
  mfileStat.setPath(path);
}

void FileProxy::setHost(const string& host) {
  mhost = host;
}

void FileProxy::setOwner(const string& owner) const {
  mfileStat.setOwner(owner);

}

void FileProxy::setGroup(const string& group) const {
  mfileStat.setGroup(group);
}

void FileProxy::setPerms(const mode_t perms) const {
mfileStat.setPerms(perms);
}

void FileProxy::setUid(const uid_t uid) const {
  mfileStat.setUid(uid);
}

void FileProxy::setGid(const gid_t gid) const {
  mfileStat.setGid(gid);
}

void FileProxy::setSize(const file_size_t size) const {
  mfileStat.setSize(size);
}

void FileProxy::setAtime(const time_t atime) const {
  mfileStat.setAtime(atime);
}

void FileProxy::setMtime(const time_t mtime) const {
  mfileStat.setMtime(mtime);
}

void FileProxy::setCtime(const time_t ctime) const {
  mfileStat.setCtime(ctime);
}

void FileProxy::setType(const FileType& type) const {
  mfileStat.setType(type);
}

void FileProxy::exists(const bool exist) const {
  this->exist = exist;
}

FileProxy::FileProxy() {
  srand(time(NULL));
}

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

FileProxy::FileProxy(const FileProxy& file) {
  srand(time(NULL));
  operator=(file);
}

const SessionProxy& FileProxy::getSession() const {
  return msessionProxy;
}

const FileStat& FileProxy::getFileStat() const{
  return mfileStat;
}


const string& FileProxy::getPath() const {
  return mfileStat.getPath();
}

const string& FileProxy::getOwner() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getOwner();
}

const string& FileProxy::getGroup() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getGroup();
}

const string& FileProxy::getHost() const {
  return mhost;
}

mode_t FileProxy::getPerms() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getPerms();
}

uid_t FileProxy::getUid() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getUid();
}

gid_t FileProxy::getGid() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getGid();
}

file_size_t FileProxy::getSize() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getSize();
}

time_t FileProxy::getAtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getAtime();
}

time_t FileProxy::getMtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getMtime();
}

time_t FileProxy::getCtime() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getCtime();
}

 FileType FileProxy::getType() const {
  if (!isUpToDate()) getInfos();
  return mfileStat.getType();
}
/*
file_host_t FileProxy::getHostType() const {
  if (getHost()=="localhost") return local;
  return remote;
}
*/


bool FileProxy::exists() const {
  if (!isUpToDate()) getInfos();
  return exist;
}

FileProxy& FileProxy::operator=(const FileProxy& file) {
  
  mfileStat= file.getFileStat();
  setHost(file.getHost());

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
