
#ifndef __SLURM_RLIMITS_INFO_H__
#define __SLURM_RLIMITS_INFO_H__


/*
 * Values for the propagate rlimits flag.
 */
#define PROPAGATE_RLIMITS    1  /* The default is to propagate rlimits */
#define NO_PROPAGATE_RLIMITS 0

struct slurm_rlimits_info {
        int  resource;          /* Values:  RLIMIT_NPROC, RLIMIT_MEMLOCK, ... */
        char *name;             /* String: "NPROC",      "MEMLOCK", ...       */
	int  propagate_flag;    /* PROPAGATE_RLIMITS or NO_PROPAGATE_RLIMITS  */
};

typedef struct slurm_rlimits_info slurm_rlimits_info_t;


extern slurm_rlimits_info_t *get_slurm_rlimits_info( void );

extern int parse_rlimits( char *rlimits_str, int propagate_flag );

#endif /*__SLURM_RLIMITS_INFO_H__*/
