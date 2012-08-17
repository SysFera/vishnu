#ifndef TOTO
#define TOTO
// Mock file for zmq items

#define ZMQ_LINGER 1
#define ZMQ_POLLIN 1
#define ZMQ_REQ 1

bool
setsockopt(int p1, int* p2, int p3){
  return true;
}

std::exception
error_t(){
}



namespace zmq {

typedef int context_t;

typedef struct error_t{
}error_t;


class message_t{
public:
  message_t(){
  }

  message_t(std::string p1){
  }

  message_t(int p1){
  }

  void*
  data(){
  }

  int
  size(){
  }

};


class socket_t{
public:
  socket_t(context_t p1, int p2){
  }
  bool
  send(message_t msg, int flag){
  }

  bool
  recv(message_t* msg, int flag){
  }

  void
  connect(const char* ){
  }

};


typedef struct pollitem_t{
public:
  zmq::socket_t s;
  int revents;
  int p1;
  int p2;
};

void
poll(pollitem_t* p1, int, int){
}



}


#endif
