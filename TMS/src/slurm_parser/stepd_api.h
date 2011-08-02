
#ifndef _STEPD_API_H
#define _STEPD_API_H

#include <inttypes.h>

#include "slurm.h"
#include "slurm_protocol_defs.h"
#include "io_hdr.h"

typedef enum {
	SLURMSTEPD_NOT_RUNNING = 0,
	SLURMSTEPD_STEP_STARTING,
	SLURMSTEPD_STEP_RUNNING,
	SLURMSTEPD_STEP_ENDING
} slurmstepd_state_t;


#endif /* _STEPD_API_H */
