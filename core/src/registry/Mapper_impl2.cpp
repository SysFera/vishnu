/**
 * \file Mapper_imp2.ccp
 * \brief Sample mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 07/02/11
 */

#include "Mapper_impl2.hpp"

Mapper_impl2::Mapper_impl2(){
};

Mapper_impl2::Mapper_impl2(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<string, string>("1", "submitJob"));
  mmap.insert (pair<string, string>("2", "submitfile"));
  mmap.insert (pair<string, string>("3", "getjobinfo"));
  mmap.insert (pair<string, string>("4", "getjobfile"));
  mmap.insert (pair<string, string>("5", "canceljob"));
};

int
Mapper_impl2::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int 
Mapper_impl2::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
Mapper_impl2::getCommand(const string& key,string& command){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key.compare(it->first)==0){
      command = it->second;
      return 0;
    }
  }
}

int
Mapper_impl2::getKey(const string& command, string& key){
  map<string, string>::const_iterator it; 
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}

int
Mapper_impl2::code(const string& cmd, unsigned int code) {
  return 0;
}
 
string
Mapper_impl2::finalize(int key){
  return "";
}
  
string
Mapper_impl2::decodeCPP (const string& msg){
  return "";
}
