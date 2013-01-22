#include "dispatcher.hpp"
#include <boost/thread.hpp>


int
main(int argc, char* argv[]) {
  using boost::this_thread::sleep;
  using boost::posix_time::seconds;

  Dispatcher dispatcher(argc, argv);

  dispatcher.run();
  // To avoid quitting too fast in case of problems
  while(1) {
    sleep(seconds(1000));
  }

  return 0;
}
