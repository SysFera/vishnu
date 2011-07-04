#include "Watcher.hpp"
#include "ToolFactory.hpp"
#include <boost/thread.hpp>

using namespace boost;

int main(int argc, char** argv){
  Watcher w(IMSVishnuTool_v1, argc, argv);
  thread thr1(bind(&Watcher::run, &w));
  thr1.join();
  while(1){
    sleep(1000);
  }
  return 0;
}
