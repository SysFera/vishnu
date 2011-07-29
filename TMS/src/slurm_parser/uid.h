
#ifndef __SLURM_UID_UTILITY_H__
#define __SLURM_UID_UTILITY_H__

#include <sys/types.h>
#include <unistd.h>

/*
 * In an ideal world, we could use sysconf(_SC_GETPW_R_SIZE_MAX) to get the
 * maximum buffer size neede for getpwnam_r(), but if there is no maximum
 * value configured, the value returned is 1024, which can too small.
 * Diito for _SC_GETGR_R_SIZE_MAX. Use 64k byte buffer by default.
 */
#define PW_BUF_SIZE 65536

/*
 * Return validated uid_t for string in ``name'' which contains
 *  either the UID number or user name
 *
 * Returns uid int uidp after verifying presence in /etc/passwd, or
 *  -1 on failure.
 */
int uid_from_string (char *name, uid_t *uidp);

/*
 * Return the primary group id for a given user id, or
 * (gid_t) -1 on failure.
 */
gid_t gid_from_uid (uid_t uid);

/*
 * Same as uid_from_name(), but for group name/id.
 */
int gid_from_string (char *name, gid_t *gidp);

/*
 * Translate uid to user name,
 * NOTE: xfree the return value
 */
char *uid_to_string (uid_t uid);

/*
 * Same as uid_to_string, but for group name.
 * NOTE: xfree the return value
 */
char *gid_to_string (gid_t gid);
#endif /*__SLURM_UID_UTILITY_H__*/
