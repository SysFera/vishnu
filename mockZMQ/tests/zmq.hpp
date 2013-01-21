#include <iostream>
#include <cstdlib>
#include <cstring>

#ifndef TOTO
#define TOTO
// Mock file for zmq items

#define ZMQ_LINGER 1
#define ZMQ_POLLIN 1
#define ZMQ_REQ 1
#define ZMQ_REP 1
#define ZMQ_ROUTER 1
#define ZMQ_DEALER 1
#define ZMQ_QUEUE 1

bool
setsockopt(int p1, int* p2, int p3);


namespace zmq {

typedef int context_t;

class error_t : public std::exception {
public:
  error_t(){}

};


class message_t{
public:
  message_t(){
    mbuff = "ok";
    msize = mbuff.length();
  }

  message_t(std::string p1){
    mbuff = "ok";
    msize = mbuff.length();
  }

  message_t(int p1){
    mbuff = "ok";
    msize = mbuff.length();
  }

  void*
  data(){
    char* s = (char *)malloc(sizeof(char)*mbuff.length());
    memcpy(s, mbuff.c_str(), mbuff.length()+1);
    return s;
  }

  int
  size(){
    return msize;
  }
private :
  int msize;
  std::string mbuff;
};


class socket_t{
public:
  socket_t(context_t p1, int p2){
  }
  bool
  send(message_t msg, int flag){
    if (maddr.compare("bad")==0){
      return false;
    }
    return true;
  }

  bool
  send(message_t& msg){
    if (maddr.compare("bad")==0){
      return false;
    }
    return true;
  }

  bool
  recv(message_t* msg){
    if (maddr.compare("bad")==0){
      return false;
    }
    return true;
  }

  bool
  recv(message_t* msg, int flag){
    if (maddr.compare("bad")==0){
      return false;
    }
    return true;
  }

  void
  connect(const char* addr){
    maddr = std::string(addr);
  }

  void
  bind(const char* ){
  }
private :
  std::string maddr;
};


typedef struct pollitem_t{
public:
  zmq::socket_t s;
  int revents;
  int p1;
  int p2;
}pollitem_t;

void
poll(pollitem_t* p1, int, int);


void
device(int p1, socket_t p2, socket_t p3);

}



#endif
