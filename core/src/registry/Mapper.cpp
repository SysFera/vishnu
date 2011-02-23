#include "Mapper.hpp"

Mapper::Mapper(MapperRegistry* reg){
  mreg = reg;
  if (pthread_mutex_init(&mutex, NULL)){
    throw new SystemException(ERRCODE_SYSTEM, "Failed to init mutex");
  }
}

Mapper::Mapper(){
  if(pthread_mutex_init(&mutex, NULL)){
    throw new SystemException(ERRCODE_SYSTEM, "Failed to init mutex");
  }
}

Mapper::~Mapper(){
  pthread_mutex_destroy(&mutex);
}

