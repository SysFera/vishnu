/**
 * \file TMSMapper.ccp
 * \brief Implementation of the TMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "TMSMapper.hpp"

TMSMapper::TMSMapper(){
};

TMSMapper::TMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(1, "submitJob"));
  mmap.insert (pair<int, string>(2, "getJobInfo"));
  mmap.insert (pair<int, string>(3, "getJobProgress"));
  mmap.insert (pair<int, string>(4, "listQueues"));
  mmap.insert (pair<int, string>(5, "listJobs"));
  mmap.insert (pair<int, string>(6, "getJobOutPut"));
  mmap.insert (pair<int, string>(7, "getAllJobsOutPut"));
  mmap.insert (pair<int, string>(8, "cancelJob"));
  mmap.insert (pair<int, string>(9, "setMachineEnv"));
  mmap.insert (pair<int, string>(10, "setMachineRefreshPeriod"));
};

int
TMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
TMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
TMSMapper::getCommand(const int& key,string& command){
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
TMSMapper::getKey(const string& command, int& key){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
  return 0;
}
