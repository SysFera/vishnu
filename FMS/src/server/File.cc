#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "File.hh"

using namespace std;

void File::setPath(const string& path) {
  this->path = path;
}

void File::setHost(const string& host) {
  this->host = host;
}

void File::setOwner(const string& owner) const {
  this->owner = owner;
}

void File::setGroup(const string& group) const {
  this->group = group;
}

void File::setPerms(const mode_t perms) const {
  this->perms = perms;
}

void File::setUid(const uid_t uid) const {
  this->uid = uid;
}

void File::setGid(const gid_t gid) const {
  this->gid = gid;
}

void File::setSize(const file_size_t size) const {
  this->size = size;
}

void File::setAtime(const time_t atime) const {
  this->atime = atime;
}

void File::setMtime(const time_t mtime) const {
  this->mtime = mtime;
}

void File::setCtime(const time_t ctime) const {
  this->ctime = ctime;
}

void File::setType(const file_type_t type) const {
  this->type = type;
}

void File::exists(const bool exist) const {
  this->exist = exist;
}

File::File() {
  srand(time(NULL));
}

File::File(const SessionServer& sessionServer,const string& path) {
  
  msessionServer=sessionServer;
 
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

File::File(const File& file) {
  srand(time(NULL));
  operator=(file);
}

const SessionServer& File::getSession() const {
  return msessionServer;
}

const string& File::getPath() const {
  return path;
}

const string& File::getOwner() const {
  if (!isUpToDate()) getInfos();
  return owner;
}

const string& File::getGroup() const {
  if (!isUpToDate()) getInfos();
  return group;
}

const string& File::getHost() const {
  return host;
}

mode_t File::getPerms() const {
  if (!isUpToDate()) getInfos();
  return perms;
}

uid_t File::getUid() const {
  if (!isUpToDate()) getInfos();
  return uid;
}

gid_t File::getGid() const {
  if (!isUpToDate()) getInfos();
  return gid;
}

file_size_t File::getSize() const {
  if (!isUpToDate()) getInfos();
  return size;
}

time_t File::getAtime() const {
  if (!isUpToDate()) getInfos();
  return atime;
}

time_t File::getMtime() const {
  if (!isUpToDate()) getInfos();
  return mtime;
}

time_t File::getCtime() const {
  if (!isUpToDate()) getInfos();
  return ctime;
}

file_type_t File::getType() const {
  if (!isUpToDate()) getInfos();
  return type;
}

file_host_t File::getHostType() const {
  if (getHost()=="localhost") return local;
  return remote;
}

bool File::exists() const {
  if (!isUpToDate()) getInfos();
  return exist;
}

File& File::operator=(const File& file) {
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
  if (pos == string::npos) return "localhost";
  return path.substr(0, pos);
}

string File::extName(const string& path) {
  size_t pos;
  
  pos = path.find_first_of(':');
  if (pos == string::npos) return path;
  return path.substr(pos+1);
}

File::~File() {}

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
