#include "dispatcher.hpp"
#include <boost/thread.hpp>


int
main(int argc, char* argv[]) {
  using boost::this_thread::sleep;
  using boost::posix_time::seconds;
  std::string confFile;

  if (argc == 2) {
    confFile = argv[1];
  }

  Dispatcher dispatcher(confFile);
  dispatcher.run();

  return 0;
}
