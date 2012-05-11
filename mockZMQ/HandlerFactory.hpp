#ifndef __HANDLERFACTORY__H__
#define __HANDLERFACTORY__H__

#include "Handler.hpp"

class HandlerFactory {

public :

  Handler*
  getHandler(std::string handlerType, boost::shared_ptr<Message> msg);

  HandlerFactory();
  ~HandlerFactory();

private:


};

#endif //  __HANDLERFACTORY__H__
