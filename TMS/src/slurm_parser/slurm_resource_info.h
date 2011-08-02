
#ifndef _RES_INFO_H
#define _RES_INFO_H

#include "slurm.h"

int slurm_verify_cpu_bind(const char *arg, char **cpu_bind,
			  cpu_bind_type_t *flags);
int slurm_verify_mem_bind(const char *arg, char **mem_bind,
			  mem_bind_type_t *flags);

#endif /* !_RES_INFO_H */
