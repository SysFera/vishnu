#ifndef __SED__H__
#define __SED__H__

#include "DIET_client.h"

// Useless class to give a root to the Server*MS classes
class SeD {
public :
virtual int
call(diet_profile_t* prof) =0;

};


#endif // __SED__H__
