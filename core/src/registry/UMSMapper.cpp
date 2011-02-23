/**
 * \file UMSMapper.ccp
 * \brief Implementation of the UMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "UMSMapper.hpp"

UMSMapper::UMSMapper(){
};

UMSMapper::~UMSMapper(){
};

UMSMapper::UMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<string, string>("1", "connect"));
  mmap.insert (pair<string, string>("2", "reconnect"));
  mmap.insert (pair<string, string>("3", "vishnu_close"));
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

int
UMSMapper::code(const string& cmd, unsigned int code){
  map<int, string>::iterator it;
  int size;
  string key;
  // If existing code -> add to the existing entry

  std::cout << "3"<< std::endl;
  if(code){
    it = mcmd.find(code);
    if (it==mcmd.end()){
      throw new SystemException(ERRCODE_SYSTEM, "Error wrong code to build command: "+cmd);
    }
    it->second += "#";
    if (cmd.compare("")==0){
      it->second += " ";
    }else {
      it->second += cmd;
    }
    return 0;
  }

  // Else creating a new unique key and insert in the map
  pthread_mutex_lock(&mutex);

  std::cout << "4"<< std::endl;
  size = mcmd.size() + 1;

  std::cout << "5"<< std::endl;
  while (true){

  std::cout << "6"<< std::endl;
    it = mcmd.find(size);
    if (it==mcmd.end()){
      break;
    }
    size++;
  }
  std::cout << "key generated in code :" << size << std::endl;
  getKey(cmd, key);
  std::cout << "command inserted :" << key << std::endl;
  mcmd.insert(pair<int, string>(size, key));
  pthread_mutex_unlock(&mutex);
  return size;
}

string
UMSMapper::finalize(int key){
  map<int, string>::iterator it;
  string res;
  pthread_mutex_lock(&mutex);
  it = mcmd.find(key);
  if (it==mcmd.end()){
    std::cout << "key not found " << key << std::endl;
    throw new SystemException(ERRCODE_SYSTEM, "Unknown key to finalize");
  }
  res = it->second;
  std::cout << "string gotten : " << res << std::endl;
  mcmd.erase(it->first);
  pthread_mutex_unlock(&mutex);
  return res;
}

string
UMSMapper::decodeCPP (const string& msg){
  // TODO
  return "";
}
