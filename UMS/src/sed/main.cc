#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerUMS.hpp"

int
usage(char* cmd) {
  std::cout << "Usage: %s <file.cfg> vishnuid vishnu_user_db_password\n"+ std::string(cmd);
  return 1;
}

int main(int argc, char* argv[], char* envp[]) {

  int res;
  
  if (argc < 4) {
    return usage(argv[0]);
  }
  
  ServerUMS server(argv[1]);
  res = server.init(argv[2], argv[3]);
  
  if (!res) {
    diet_print_service_table();
    res = diet_SeD(argv[1], argc, argv);
  } else {
    std::cout << "There was a problem during services initialization" << std::endl;
  }
  
 return res; 
}