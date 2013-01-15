
#include <iostream>
#include <cstdio>


#include "TmsPosixClient.hpp"

using namespace std;


int
main(int argc, char* argv[]){
  int result;
  struct st_job resultat;

  if (argc < 2) {
    cerr << "Usage : " << argv[0] << " command" << endl;
    return 1;
  }
  
  result = ReqSubmit(argv[1],&resultat);
  if (result < 0) {
    cerr << "Erreur : "<< result << endl;
    perror("Sending request error");
    return 2;
  }

  printf("JobId:%s\n",resultat.JobId);
  return 0;
}
