#include "MapperRegistry.hpp"

using namespace std;

MapperRegistry* MapperRegistry::mreg = NULL;

MapperRegistry::MapperRegistry(){
  mmapper = map<string, Mapper*>();
}

MapperRegistry::~MapperRegistry(){
}

MapperRegistry*
MapperRegistry::getInstance(){
  if (!mreg){
    std::cout << "mapper create" << std::endl;
    mreg = new MapperRegistry();
  }
  std::cout << "existing mapper " << std::endl;
  return mreg;
}

int
MapperRegistry::addMapper(string s, Mapper* mapper){
  if (mreg==NULL){
    me = SystemException(ERRCODE_SYSTEM, "Cannot add to a nill mapper");
    me.setType(VishnuException::INTERNAL);
    me.setMsg(6);
    me.setMsgComp("Error null registry");
    throw(me);
  }
  if (!contains(s)){
//    std::cout << "adding : "<< mapper << std::endl;
    mmapper.insert( pair<string, Mapper*>(s, mapper));
  }
  display();
  return 0;
}

int
MapperRegistry::removeMapper(string s){
  if (!mreg){
    return 0;
  }
  if (contains(s)){
    mmapper.erase(s);
  }
  return 0;
}

Mapper*
MapperRegistry::getMapper(const string& s) {
  map<string, Mapper*>::const_iterator it;
  for (it = mmapper.begin() ; it != mmapper.end() ; it++){
    if (s.compare(it->first)==0){
      std::cout << "mapper found " << std::endl;
//      *mapper = it->second;
      return it->second;
    }
  }
  std::cout << "mapper not found " << std::endl;
  me = SystemException(ERRCODE_SYSTEM, "Mapper not found");
  me.setType(VishnuException::INTERNAL);
  me.setMsg(6);
  me.setMsgComp("Error invalid mapper");
  display();
  throw(me);
  return 0;
}

bool
MapperRegistry::contains(string s){
  map<string, Mapper*>::const_iterator it = mmapper.begin();
  for ( ; it != mmapper.end() ; it++){
    if (s.compare(it->first)==0){
      return true;
    }
  }
  return false;
}

void MapperRegistry::display(){
  map<string, Mapper*>::const_iterator it;
  for (it = mmapper.begin() ; it != mmapper.end() ; it++){
//    cout << "key = " << it->first << " and val = " << it->second << endl;
  }
  cout << "<------------------------------------------------->"<<endl;
}
