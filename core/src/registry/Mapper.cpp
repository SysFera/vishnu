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

