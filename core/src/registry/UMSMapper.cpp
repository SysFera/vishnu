/**
 * \file UMSMapper.ccp
 * \brief Implementation of the UMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "UMSMapper.hpp"

UMSMapper::UMSMapper(){
};

UMSMapper::UMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<string, string>("1", "connect"));
  mmap.insert (pair<string, string>("2", "reconnect"));
  mmap.insert (pair<string, string>("3", "close"));
  mmap.insert (pair<string, string>("4", "addVishnuUser"));
  mmap.insert (pair<string, string>("5", "updateVishnuUser"));
  mmap.insert (pair<string, string>("6", "deleteVishnuUser"));
  mmap.insert (pair<string, string>("7", "changePassword"));
  mmap.insert (pair<string, string>("8", "resetPassword"));
  mmap.insert (pair<string, string>("9", "addLocalAccount"));
  mmap.insert (pair<string, string>("10", "updateLocalAccount"));
  mmap.insert (pair<string, string>("11", "deleteLocalAccount"));
  mmap.insert (pair<string, string>("12", "saveConfiguration"));
  mmap.insert (pair<string, string>("13", "retoreConfiguration"));
  mmap.insert (pair<string, string>("14", "addMachine"));
  mmap.insert (pair<string, string>("15", "updateMachine"));
  mmap.insert (pair<string, string>("16", "deleteMachine"));
  mmap.insert (pair<string, string>("17", "listLocalAccount"));
  mmap.insert (pair<string, string>("18", "listMachine"));
  mmap.insert (pair<string, string>("19", "listHistoryCmd"));
  mmap.insert (pair<string, string>("20", "listOptions"));
  mmap.insert (pair<string, string>("21", "listUsers"));
  mmap.insert (pair<string, string>("22", "listSessions"));
  mmap.insert (pair<string, string>("23", "configureDefaultOption"));
  mmap.insert (pair<string, string>("24", "configureOption"));
};

int
UMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int 
UMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
UMSMapper::getCommand(const string& key,string& command){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key.compare(it->first)==0){
      command = it->second;
      return 0;
    }
  }
}

int
UMSMapper::getKey(const string& command, string& key){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}
