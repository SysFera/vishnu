#include <errno.h>

#define ERRMSG(msg) fprintf(stderr, "Error line %d in file %s with message :\n %s \n", __LINE__, __FILE__, msg);
