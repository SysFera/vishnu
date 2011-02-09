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
    mreg = new MapperRegistry();
  }
  return mreg;
}

int
MapperRegistry::addMapper(string s, Mapper* mapper){
  if (!mreg){
    e = InternalVishnuException();
    e.setType(VishnuException::INTERNAL);
    e.setMsg(6);
    e.setMsgComp("Error null registry");
  }
  if (!contains(s)){
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

int 
MapperRegistry::getMapper(const string& s, Mapper** mapper){
  map<string, Mapper*>::const_iterator it; 
  for (it = mmapper.begin() ; it != mmapper.end() ; it++){
    if (s.compare(it->first)==0){
      *mapper = it->second;
      return 0;
    }
  }
  e = InternalVishnuException();
  e.setType(VishnuException::INTERNAL);
  e.setMsg(6);
  e.setMsgComp("Error invalid mapper");
  display();
  throw(e);
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
    cout << "key = " << it->first << " and val = " << it->second << endl;
  }
  cout << "<------------------------------------------------->"<<endl;
}
