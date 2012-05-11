#ifndef __BASICHANDLER__H__
#define __BASICHANDLER__H__

#include "Annuary.hpp"
#include "Message.hpp"
#include "TreatmentData.hpp"

class BasicHandler : public Handler {

public :
  BasicHandler(boost::shared_ptr<Message> msg);
  virtual ~BasicHandler();


  template <class T>
  boost::shared_ptr<Message>
  treat(TreatmentData<T>* data);

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

private:

};


#endif // __BASICHANDLER__H__
