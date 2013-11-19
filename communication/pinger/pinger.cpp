#include "DIET_client.h"

int 
main(int argc, char** argv){
  diet_profile_t* profile = NULL;
  std::string msg;
  if (argc<2){
    std::cout << "usage : " << argv[0] << " <server> " << std::endl;
    exit(-1);
  }
  std::string name(argv[1]);
  profile = diet_profile_alloc(std::string("heartbeat")+name,0,0,1);
  diet_initialize("/home/keo/Desktop/vishnu/buildZMQ/vishnu-sample.cfg", 0, NULL);
  if(diet_call(profile)){
    std::cout << "Failed to ping " << std::endl;
    exit(-1);
  }
  diet_string_get(profile,1, msg);
  std::cout << "Heartbeat success " << msg << std::endl;

}
