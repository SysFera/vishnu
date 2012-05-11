#ifndef __HANDLER__H__
#define __HANDLER__H__

typedef enum {
  GEAD=0, // Get Address
  DOSE,   // Do Service
  ACDO,   // Action Done
  ADSE,   // Add Server
  RESE,   // Remove Server
  HAER   // Has Error
} MsgType;

class Message;
class Annuary;

#include <string>
#include <boost/shared_ptr.hpp>

class Handler {

public:

  Handler(boost::shared_ptr<Message> msg);

  virtual ~Handler();

  virtual boost::shared_ptr<Message>
  getAddress(Annuary* ann)=0;

  virtual boost::shared_ptr<Message>
  actionDone()=0;

  virtual boost::shared_ptr<Message>
  addServer(Annuary* ann)=0;

  virtual boost::shared_ptr<Message>
  removeServer(Annuary* ann)=0;

private:
protected :
  boost::shared_ptr<Message> mmsg;

};

#endif // __HANDLER__H__
