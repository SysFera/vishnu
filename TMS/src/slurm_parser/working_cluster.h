
#ifndef _WORKINGCLUSTER_H
#define _WORKINGCLUSTER_H

/* Return the number of dimensions in the current working cluster */
extern uint16_t slurmdb_setup_cluster_dims(void);

/* Return the architecture flags in the current working cluster */
extern uint32_t slurmdb_setup_cluster_flags(void);

/* Translate architecture flag strings to their equivalent bitmaps */
extern uint32_t slurmdb_str_2_cluster_flags(char *flags_in);

/*
 * Translate architecture flag bitmaps to their equivalent comma-delimited
 * string
 *
 * NOTE: Call xfree() to release memory allocated to the return value
 */
extern char *slurmdb_cluster_flags_2_str(uint32_t flags_in);

#endif
