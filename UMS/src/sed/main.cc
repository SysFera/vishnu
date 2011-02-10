#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerUMS.hh"

int
usage(char* cmd) {
  std::cout << "Usage: %s <file.cfg> vishnuid\n"+ std::string(cmd);
  return 1;
}


int main(int argc, char* argv[], char* envp[]) {

  int res;
  
  if (argc < 3) {
    return usage(argv[0]);
  }
  
  ServerUMS server(argv[1], argv[2]);
  server.init();
  
  diet_print_service_table();
  res = diet_SeD(argv[1], argc, argv);
  
 return res; 
}