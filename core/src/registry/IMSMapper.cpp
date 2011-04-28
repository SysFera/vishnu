/**
 * \file IMSMapper.ccp
 * \brief Implementation of the IMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "IMSMapper.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;

IMSMapper::IMSMapper(){
};

IMSMapper::IMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(1, "exportCommands"));
  mmap.insert (pair<int, string>(2, "getMetricCurrentValue"));
  mmap.insert (pair<int, string>(3, "getMetricHistory"));
  mmap.insert (pair<int, string>(4, "getProcesses"));
  mmap.insert (pair<int, string>(5, "setSystemInfo"));
  mmap.insert (pair<int, string>(6, "setSystemThreshold"));
  mmap.insert (pair<int, string>(7, "getSystemThreshold"));
  mmap.insert (pair<int, string>(8, "defineUserIdentifier"));
  mmap.insert (pair<int, string>(9, "defineMachineIdentifier"));
  mmap.insert (pair<int, string>(10, "defineJobIdentifier"));
  mmap.insert (pair<int, string>(11, "defineTransferIdentifier"));
  mmap.insert (pair<int, string>(12, "loadShed"));
  mmap.insert (pair<int, string>(13, "setUpdateFrequency"));
  mmap.insert (pair<int, string>(14, "getUpdateFrequency"));
};

int
IMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
IMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
IMSMapper::getCommand(const int& key,string& command){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key==it->first){
      command = it->second;
      return 0;
    }
  }
  return 0;
}

int
IMSMapper::getKey(const string& command, int& key){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
  return 0;
}

int
IMSMapper::code(const string& cmd, unsigned int code){
  map<int, string>::iterator it;
  int size;
  string key;
  int keycode;
  // If existing code -> add to the existing entry
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
  size = mcmd.size() + 1;
  while (true){
    it = mcmd.find(size);
    if (it==mcmd.end()){
      break;
    }
    size++;
  }
  getKey(cmd, keycode);
  key = convertToString(keycode);
  mcmd.insert(pair<int, string>(size, key));
  pthread_mutex_unlock(&mutex);
  return size;
}
