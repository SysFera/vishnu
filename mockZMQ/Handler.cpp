#include "Handler.hpp"

Handler::Handler(boost::shared_ptr<Message> msg) : mmsg(msg){
}

Handler::~Handler(){
}

