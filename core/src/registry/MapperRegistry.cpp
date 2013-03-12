#include "MapperRegistry.hpp"

#include <map>

using namespace std;


MapperRegistry* MapperRegistry::mreg = NULL; //%RELAX<MISRA_0_1_3> Used in this file

MapperRegistry::MapperRegistry() {}

MapperRegistry::~MapperRegistry() {}

MapperRegistry*
MapperRegistry::getInstance(){
  if (!mreg){
    mreg = new MapperRegistry();
  }
  return mreg;
}

int
MapperRegistry::addMapper(const std::string& s, Mapper* mapper) {
  if (!mreg) {
    me = SystemException(ERRCODE_SYSTEM, "Cannot add to a nill mapper");
    me.setType(VishnuException::INTERNAL);
    me.setMsg(6);
    me.setMsgComp("Error null registry");
    throw(me);
  }
  if (!contains(s)){
    mmapper[s] = mapper;
  }
  display();
  return 0;
}

int
MapperRegistry::removeMapper(const std::string& s) {
  // contains(s) is only evaluated if mreg != NULL
  if (!mreg && contains(s)){
    mmapper.erase(s);
  }
  return 0;
}

Mapper*
MapperRegistry::getMapper(const std::string& s) {
  if (contains(s)) {
    return mmapper[s];
  }
  me = SystemException(ERRCODE_SYSTEM, "Mapper not found");
  me.setType(VishnuException::INTERNAL);
  me.setMsg(6);
  me.setMsgComp("Error invalid mapper");
  throw(me);
  return NULL;
}

bool
MapperRegistry::contains(const std::string& s) {
  std::map<std::string, Mapper*>::const_iterator it = mmapper.find(s);
  return (it != mmapper.end());
}

void
MapperRegistry::display() {
  map<std::string, Mapper*>::const_iterator it;
  for (it = mmapper.begin() ; it != mmapper.end() ; ++it){
  }
}
