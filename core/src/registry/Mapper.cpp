#include "Mapper.hpp"

Mapper::Mapper(MapperRegistry* reg){
  mreg = reg;
  /*pthread_mutex_init(&mutex, NULL);*/
  mutex.initialize();
}

Mapper::Mapper(){
  mreg = NULL;
  /*pthread_mutex_init(&mutex, NULL);*/
  mutex.initialize();
}

Mapper::Mapper(const Mapper& m){
 *this = m;
}

Mapper & Mapper::operator = ( const Mapper & m )
{
  if( this != &m ){
    delete this->mreg;
    this->mreg = m.mreg;
    this->mmap = m.mmap;
    this->mname = m.mname;
    this->mcmd = m.mcmd;
    this->mutex.initialize();
  }
 
  return *this;
}

Mapper::~Mapper(){
  /*pthread_mutex_destroy(&mutex);*/
  mutex.destroy();
}


string
Mapper::finalize(int key){
  map<int, string>::iterator it;
  string res;
  /*pthread_mutex_lock(&mutex);*/
  mutex.lock();
  it = mcmd.find(key);
  if (it==mcmd.end()){
    throw SystemException(ERRCODE_SYSTEM, "Unknown key to finalize");
  }
  res = it->second;
  mcmd.erase(it->first);
  /*pthread_mutex_unlock(&mutex);*/
  mutex.unlock();
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
