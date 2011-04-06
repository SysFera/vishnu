
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

extern "C" {
 #include "pbs_error.h"
 #include "pbs_ifl.h"
}

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define notNULL(x) (((x)!=NULL) && (strlen(x)>(size_t)0))
#define NULLstr(x) (((x)==NULL) || (strlen(x)==0))

#define MAX_LINE_LEN 32768  /* increased from 2048 */
#define MAXSERVERNAME PBS_MAXSERVERNAME+PBS_MAXPORTNUM+2
#define PBS_DEPEND_LEN 65528  /* increased from 2040 */


int cnt2server(char *server);
