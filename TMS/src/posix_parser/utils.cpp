
#include <string.h>

int
SetProcName(int argc, char* argv[],const char* Name) {
  int lgbufargs=0;

  for (int i=0;i<argc;i++) {
    lgbufargs += strlen(argv[i])+1;
  };
  
  // Set Procname  
  argv[1]=NULL;
  memset(argv[0],0,lgbufargs);
  strncpy(argv[0],Name,lgbufargs-1);
  
  return 0;
}

char*
GetProcName(char* argv0) {
  char* tmp;

  if ((tmp = strrchr(argv0, '/')) != NULL)
    tmp++;
  else
    tmp = argv0;

  return tmp;
}

