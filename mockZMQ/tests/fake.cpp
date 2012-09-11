#include "zmq.hpp"

bool
setsockopt(int p1, int* p2, int p3){
  return true;
}

namespace zmq {


void
poll(pollitem_t* p1, int, int){
  p1[0].revents = ZMQ_POLLIN;
}

void
device(int p1, socket_t p2, socket_t p3){
}


}
