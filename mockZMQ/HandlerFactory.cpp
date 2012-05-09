#include "HandlerFactory.hpp"

Handler*
HandlerFactory::getHandler(string type, boost::shared_ptr<Message> msg){
  Handler* handl;
  switch(vishnu::convertToInt(type)){
  default:
    handl = new BasicHandler(msg);
  }
  return handl;
}


HandlerFactory::HandlerFactory(){
}

HandlerFactory::~HandlerFactory(){
}

