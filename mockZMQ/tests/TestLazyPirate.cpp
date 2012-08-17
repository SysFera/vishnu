#include <iostream>
#include <vector>
#include "zmq.hpp"
#include "zhelpers.hpp"

class LazyPirateClientTest {
public:
  LazyPirateClientTest():ctxt(0), addr("tcp://localhost:5555"){
  }

  ~LazyPirateClientTest(){
  }

  void
  testSend(){
    LazyPirateClient lp(ctxt, addr);
    if (!lp.send("bonjour")){
      std::cerr << "Fail test send " << std::endl;
    }
  }
  void
  testSendBadAddr(){
    LazyPirateClient lp (ctxt, "1"+addr);
    if (!lp.send("bonjour")){
      std::cerr << "Fail test send bad addr " << std::endl;
    }
  }

  void
  testRecv(){
    LazyPirateClient lp (ctxt, addr);
    lp.send("bonjour");
    if (lp.recv().compare("ok") != 0){
      std::cerr << "Fail test recv " << std::endl;
    }
  }

  void
  testRecvBadAddr(){
    LazyPirateClient lp (ctxt, "1"+addr);
    lp.send("bonjour");
    if (lp.recv().compare("ok") == 0){
      std::cerr << "Fail test recv bad addr " << std::endl;
    }
  }

private:
  std::string addr;
  zmq::context_t ctxt;
};





int main(int argc, char** argv){
  LazyPirateClientTest test;
  test.testSend();
  test.testSendBadAddr();
  test.testRecv();
  test.testRecvBadAddr();
  return 0;
}
