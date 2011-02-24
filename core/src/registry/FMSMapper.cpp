/**
 * \file FMSMapper.ccp
 * \brief Implementation of the FMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "FMSMapper.hpp"

FMSMapper::FMSMapper(){
};

FMSMapper::FMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(1, "createFile"));
  mmap.insert (pair<int, string>(2, "createDir"));
  mmap.insert (pair<int, string>(3, "removeFile"));
  mmap.insert (pair<int, string>(4, "removeDir"));
  mmap.insert (pair<int, string>(5, "chGrp"));
  mmap.insert (pair<int, string>(6, "chMod"));
  mmap.insert (pair<int, string>(7, "headOfFile"));
  mmap.insert (pair<int, string>(8, "tailOfFile"));
  mmap.insert (pair<int, string>(9, "contentOfFile"));
  mmap.insert (pair<int, string>(10, "listDir"));
  mmap.insert (pair<int, string>(11, "copyFile"));
  mmap.insert (pair<int, string>(12, "copyAsyncFile"));
  mmap.insert (pair<int, string>(13, "moveFile"));
  mmap.insert (pair<int, string>(14, "moveAsyncFile"));
  mmap.insert (pair<int, string>(15, "listFileTransferStatus"));
  mmap.insert (pair<int, string>(16, "listFileTransfers"));
  mmap.insert (pair<int, string>(17, "stopFileTransfer"));
  mmap.insert (pair<int, string>(18, "getFileInfo"));
};

int
FMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
FMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
FMSMapper::getCommand(const int& key,string& command){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key==it->first){
      command = it->second;
      return 0;
    }
  }
}

int
FMSMapper::getKey(const string& command, int& key){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}
