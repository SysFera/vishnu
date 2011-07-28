
#ifndef _RES_INFO_H
#define _RES_INFO_H

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif			/* HAVE_INTTYPES_H */
#endif

int slurm_get_avail_procs(const uint16_t socket_cnt,
			  const uint16_t core_cnt,
			  const uint16_t thread_cnt,
			  const uint16_t cpuspertask,
			  const uint16_t ntaskspernode,
			  const uint16_t ntaskspersocket,
			  const uint16_t ntaskspercore,
			  uint16_t *cpus,
			  uint16_t *sockets,
			  uint16_t *cores,
			  uint16_t *threads,
			  const uint16_t *alloc_cores,
			  const uint16_t cr_type,
			  uint32_t job_id, char *name);

void slurm_print_cpu_bind_help(void);
void slurm_print_mem_bind_help(void);

void slurm_sprint_cpu_bind_type(char *str, cpu_bind_type_t cpu_bind_type);
void slurm_sprint_mem_bind_type(char *str, mem_bind_type_t mem_bind_type);

int slurm_verify_cpu_bind(const char *arg, char **cpu_bind,
			  cpu_bind_type_t *flags);
int slurm_verify_mem_bind(const char *arg, char **mem_bind,
			  mem_bind_type_t *flags);

#endif /* !_RES_INFO_H */
