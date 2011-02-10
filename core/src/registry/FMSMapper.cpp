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
  mmap.insert (pair<string, string>("1", "createFile"));
  mmap.insert (pair<string, string>("2", "createDir"));
  mmap.insert (pair<string, string>("3", "removeFile"));
  mmap.insert (pair<string, string>("4", "removeDir"));
  mmap.insert (pair<string, string>("5", "chGrp"));
  mmap.insert (pair<string, string>("6", "chMod"));
  mmap.insert (pair<string, string>("7", "headOfFile"));
  mmap.insert (pair<string, string>("8", "tailOfFile"));
  mmap.insert (pair<string, string>("9", "contentOfFile"));
  mmap.insert (pair<string, string>("10", "listDir"));
  mmap.insert (pair<string, string>("11", "copyFile"));
  mmap.insert (pair<string, string>("12", "copyAsyncFile"));
  mmap.insert (pair<string, string>("13", "moveFile"));
  mmap.insert (pair<string, string>("14", "moveAsyncFile"));
  mmap.insert (pair<string, string>("15", "listFileTransferStatus"));
  mmap.insert (pair<string, string>("16", "listFileTransfers"));
  mmap.insert (pair<string, string>("17", "stopFileTransfer"));
  mmap.insert (pair<string, string>("18", "getFileInfo"));
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
FMSMapper::getCommand(const string& key,string& command){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key.compare(it->first)==0){
      command = it->second;
      return 0;
    }
  }
}

int
FMSMapper::getKey(const string& command, string& key){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}
