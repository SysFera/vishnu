#include "Dispatcher.hpp"
#include <boost/thread.hpp>


int
main(int argc, char* argv[]) {
  using boost::this_thread::sleep;
  using boost::posix_time::seconds;
  std::string confFile;

  if (argc == 2) {
    confFile = argv[1];
  }
  try{
    Dispatcher dispatcher(confFile);
    dispatcher.run();
  } catch (VishnuException &e){
    std::cout << "[Error] : An error occured when initializing the dispatcher. Starting with default parameters. \n The error was: " << e.what() << "\n\n";
    Dispatcher dispatcher("");
    dispatcher.run();
  }

  return 0;
}
