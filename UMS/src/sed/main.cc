#include "ServerUMS.hh"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
usage(char* cmd)
{
  printf("Usage: %s <file.cfg>\n", cmd);
  return 1;
}


int main(int argc, char* argv[], char* envp[]) {

  int res;
  
  if (argc < 2) {
    return usage(argv[0]);
  }


  ServerUMS server(argv[1]);
  
  
  //diet_profile_desc_free(server.profile);
  //diet_print_service_table();
  res = diet_SeD(argv[1], argc, argv);
  
 return res; 
}