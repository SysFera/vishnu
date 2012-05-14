#ifndef __BASICHANDLER__H__
#define __BASICHANDLER__H__

#include "Annuary.hpp"
#include "Message.hpp"
#include "TreatmentData.hpp"

class BasicHandler : public Handler {

public :
  BasicHandler(boost::shared_ptr<Message> msg);
  virtual ~BasicHandler();


  virtual boost::shared_ptr<Message>
  treat(TreatmentData* data = NULL);

  virtual boost::shared_ptr<Message>
  getAddress(Annuary* ann);
  virtual boost::shared_ptr<Message>
  actionDone();
  template <class T>
  boost::shared_ptr<Message>
  doService(T* server);
  virtual boost::shared_ptr<Message>
  addServer(Annuary* ann);
  virtual boost::shared_ptr<Message>
  removeServer(Annuary* ann);

// Send a message and get the answer
  virtual boost::shared_ptr<Message>
  send(zmq::socket_t& socket);

private:

};


#endif // __BASICHANDLER__H__
