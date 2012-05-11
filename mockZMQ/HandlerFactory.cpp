#include "HandlerFactory.hpp"

#include "Handler.hpp"
#include "utilVishnu.hpp"
#include "BasicHandler.hpp"

Handler*
HandlerFactory::getHandler(std::string handlerType, boost::shared_ptr<Message> msg){
  Handler* handl;
  switch(vishnu::convertToInt(handlerType)){
  default:
    handl = new BasicHandler(msg);
  }
  return handl;
}


HandlerFactory::HandlerFactory(){
}

HandlerFactory::~HandlerFactory(){
}

