/**
 * \file Mapper_imp.ccp
 * \brief Sample mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */

#include "Mapper_impl.hpp"

Mapper_impl::Mapper_impl(){
};

Mapper_impl::Mapper_impl(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<string, string>("1", "connect"));
  mmap.insert (pair<string, string>("2", "reconnect"));
  mmap.insert (pair<string, string>("3", "close"));
  mmap.insert (pair<string, string>("4", "addVishnuUser"));
  mmap.insert (pair<string, string>("5", "addLocalAccount"));
};

int
Mapper_impl::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int 
Mapper_impl::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
Mapper_impl::getCommand(const string& key,string& command){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key.compare(it->first)==0){
      command = it->second;
      return 0;
    }
  }
}

int
Mapper_impl::getKey(const string& command, string& key){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}
