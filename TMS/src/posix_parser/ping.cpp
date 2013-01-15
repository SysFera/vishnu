
#include <iostream>
#include <cstdio>
#include <string.h>


#include "TmsPosixClient.hpp"

using namespace std;


int
main(int argc, char* argv[]){
  int result;
  char resul[255];

  if (argc < 2) {
    cerr << "Usage : " << argv[0] << " string " << endl;
    return 1;
  }

  memset(resul,0,255);
  
  cout << argv[1] << endl;

  result = ReqEcho(argv[1],resul);

  if (result < 0) {
    cerr << "Erreur : "<< result << endl;
    perror("Sending request error");
    return 2;
  }
  cout << resul << endl;

  return 0;
}
