
#include <iostream>
#include <cstdio>
#include <string.h>


#include "TmsPosixClient.hpp"

using namespace std;


int
main(int argc, char* argv[]){
  int result;
  struct st_job resul;

  if (argc < 2) {
    cerr << "Usage : " << argv[0] << " string " << endl;
    return 1;
  }

  cout << argv[1] << endl;

  result = ReqInfo(argv[1],&resul);

  if (result < 0) {
    cerr << "Erreur : "<< result << endl;
    perror("Sending request error");
    return 2;
  }
  cout << "[" << resul.JobId << "] :" << endl;

  return 0;
}
