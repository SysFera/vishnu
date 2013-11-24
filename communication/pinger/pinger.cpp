#include "DIET_client.h"

int 
main(int argc, char** argv){
  diet_profile_t* profile = NULL;
  std::string msg;

  if (argc<3 ||
      strcmp(argv[1],"-h") ==0){
    std::cout << "usage : " << argv[0] << " <confFile> <server> [mid] " << std::endl;
    exit(-1);
  }
  std::string name((argc<3?"":argv[2]));
  std::string mid((argc<4?"":argv[3]));
  profile = diet_profile_alloc(std::string("heartbeat")+name+mid,0,0,1);
  diet_initialize(argv[1], 0, NULL);
  if(diet_call(profile)){
    std::cout << "\nFailed to ping " << std::endl;
    exit(-1);
  }
  diet_string_get(profile,1, msg);
  std::cout << "Heartbeat success " << msg << std::endl;

}
