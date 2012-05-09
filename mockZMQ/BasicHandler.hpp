#ifndef __BASICHANDLER__H__
#define __BASICHANDLER__H__

#include "Message.hpp"

class BasicHandler {

public :
  BasicHandler(boost::shared_ptr<Message> msg);
  ~BasicHandler();

  void
  treat();

private:

};


#endif // __BASICHANDLER__H__
