#ifndef __HANDLERFACTORY__H__
#define __HANDLERFACTORY__H__

class HandlerFactory {

public :

  Handler*
  getHandler(string type);

  HandlerFactory();
  ~HandlerFactory();

private:


};

#endif //  __HANDLERFACTORY__H__
