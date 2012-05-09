#ifndef __HANDLER__H__
#define __HANDLER__H__

class Handler {

public:
  Handler(boost::shared_ptr<Message> msg);
  virtual ~Handler();

  virtual void
  treat() = 0;
private:
  boost::shared_ptr<Message> mmsg;

};

#endif // __HANDLER__H__
