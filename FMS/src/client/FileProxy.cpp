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


void
FileProxy::setFileStat(const FileStat& fileStat) const {
  mfileStat = fileStat;
}


void
FileProxy::setPath(const string& path) {
  mfileStat.setPath(path);
}


void
FileProxy::setHost(const string& host) {
  mhost = host;
}


void
FileProxy::exists(const bool exist) const {
  this->exist = exist;
}


FileProxy::FileProxy() : exist(false) {}


FileProxy::FileProxy(const SessionProxy& sessionProxy, const string& path)
  : msessionProxy(sessionProxy) {
  size_t pos = path.find(':');

  if (pos == string::npos) {
    setPath(path);
    setHost("localhost");
  } else {
    setPath(path.substr(pos+1));
    setHost(path.substr(0,pos));
  }
}


FileProxy::FileProxy(const FileProxy& file) {
  operator=(file);
}


const SessionProxy&
FileProxy::getSession() const {
  return msessionProxy;
}


const FileStat&
FileProxy::getFileStat() const{
  return mfileStat;
}


const string&
FileProxy::getPath() const {
  return mfileStat.getPath();
}


const string&
FileProxy::getHost() const {
  return mhost;
}


bool
FileProxy::exists() const {
  if (!isUpToDate()){
    getInfos();
  }
  return exist;
}


FileProxy&
FileProxy::operator=(const FileProxy& fileProxy) {
  mfileStat= fileProxy.getFileStat();
  setHost(fileProxy.getHost());

  exists(fileProxy.exists());

  return *this;
}


string
FileProxy::extHost(const string& path) {
  size_t pos;

  pos = path.find_first_of(':');
  if (pos == string::npos) {
    return "localhost";
  }
  return path.substr(0, pos);
}


string
 FileProxy::extName(const string& path) {
  size_t pos;

  pos = path.find_first_of(':');

  if (pos == string::npos){
    return path;
  }
  return path.substr(pos+1);
}


FileProxy::~FileProxy() {}
