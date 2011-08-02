
#ifndef _JOB_OPTIONS_H
#define _JOB_OPTIONS_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

//#include "pack.h"

typedef struct job_options * job_options_t;	/* opaque data type */

struct job_option_info {
	int type;
	char *option;
	char *optarg;
};


#endif /* !_JOB_OPTIONS_H */
