#include "Mapper.hpp"

Mapper::Mapper(MapperRegistry* reg){
  mreg = reg;
  if (pthread_mutex_init(&mutex, NULL)){
    throw SystemException(ERRCODE_SYSTEM, "Failed to init mutex");
  }
}

Mapper::Mapper(){
  if(pthread_mutex_init(&mutex, NULL)){
    throw  SystemException(ERRCODE_SYSTEM, "Failed to init mutex");
  }
}

Mapper::Mapper(const Mapper& m){
 *this = m;
}
Mapper::~Mapper(){
  pthread_mutex_destroy(&mutex);
}


string
Mapper::finalize(int key){
  map<int, string>::iterator it;
  string res;
  pthread_mutex_lock(&mutex);
  it = mcmd.find(key);
  if (it==mcmd.end()){
    throw new SystemException(ERRCODE_SYSTEM, "Unknown key to finalize");
  }
  res = it->second;
  mcmd.erase(it->first);
  pthread_mutex_unlock(&mutex);
  return res;
}

void
Mapper::findSeparator(const string& s, vector<int>& vec){
  size_t cpt = 0;
  do{
    cpt=s.find_first_of("#", cpt);
    if (cpt ==string::npos){
      return;
    }
    vec.push_back(cpt);
    cpt++;
  }
  while ((true));
}
