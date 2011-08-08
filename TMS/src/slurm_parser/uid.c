
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "uid.h"
#include "xmalloc.h"
#include "xstring.h"

static int _getpwnam_r (const char *name, struct passwd *pwd, char *buf,
		size_t bufsiz, struct passwd **result)
{
	int rc;
	while (1) {
		rc = getpwnam_r(name, pwd, buf, bufsiz, result);
		if (rc == EINTR)
			continue;
		if (rc != 0)
			*result = NULL;
		break;
	}
	return (rc);
}

static int _getpwuid_r (uid_t uid, struct passwd *pwd, char *buf,
		size_t bufsiz, struct passwd **result)
{
	int rc;
	while (1) {
		rc = getpwuid_r(uid, pwd, buf, bufsiz, result);
		if (rc == EINTR)
			continue;
		if (rc != 0)
			*result = NULL;
		break;
	}
	return rc;
}

int
uid_from_string (char *name, uid_t *uidp)
{
	struct passwd pwd, *result;
	char buffer[PW_BUF_SIZE], *p = NULL;
	long l;

	/*
	 *  Check to see if name is a valid username first.
	 */
	if ((_getpwnam_r (name, &pwd, buffer, PW_BUF_SIZE, &result) == 0)
	    && result != NULL) {
		*uidp = result->pw_uid;
		return 0;
	}

	/*
	 *  If username was not valid, check for a valid UID.
	 */
	errno = 0;
	l = strtol (name, &p, 10);
	if (((errno == ERANGE) && ((l == LONG_MIN) || (l == LONG_MAX)))
	   || (name == p)
	   || (*p != '\0')
	   || (l < 0)
	   || (l > INT_MAX))
		return -1;

	/*
	 *  Now ensure the supplied uid is in the user database
	 */
	if (_getpwuid_r (l, &pwd, buffer, PW_BUF_SIZE, &result) != 0)
		return -1;

	*uidp = (uid_t) l;
	return 0;
}

char *
uid_to_string (uid_t uid)
{
	struct passwd pwd, *result;
	char buffer[PW_BUF_SIZE], *ustring;
	int rc;

	/* Suse Linux does not handle multiple users with UID=0 well */
	if (uid == 0)
		return slurm_xstrdup("root");

	rc = _getpwuid_r (uid, &pwd, buffer, PW_BUF_SIZE, &result);
	if (result && (rc == 0))
		ustring = slurm_xstrdup(result->pw_name);
	else
		ustring = slurm_xstrdup("nobody");
	return ustring;
}

gid_t
gid_from_uid (uid_t uid)
{
	struct passwd pwd, *result;
	char buffer[PW_BUF_SIZE];
	gid_t gid;
	int rc;

	rc = _getpwuid_r(uid, &pwd, buffer, PW_BUF_SIZE, &result);
	if (result && (rc == 0))
		gid = result->pw_gid;
	else
		gid = (gid_t) -1;

	return gid;
}

static int _getgrnam_r (const char *name, struct group *grp, char *buf,
		size_t bufsiz, struct group **result)
{
	int rc;
	while (1) {
		rc = getgrnam_r (name, grp, buf, bufsiz, result);
		if (rc == EINTR)
			continue;
		if (rc != 0)
			*result = NULL;
		break;
	}
	return (rc);
}

static int _getgrgid_r (gid_t gid, struct group *grp, char *buf,
		size_t bufsiz, struct group **result)
{
	int rc;
	while (1) {
		rc = getgrgid_r (gid, grp, buf, bufsiz, result);
		if (rc == EINTR)
			continue;
		if (rc != 0)
			*result = NULL;
		break;
	}
	return rc;
}

int
gid_from_string (char *name, gid_t *gidp)
{
	struct group grp, *result;
	char buffer[PW_BUF_SIZE], *p = NULL;
	long l;

	/*
	 *  Check for valid group name first.
	 */
	if ((_getgrnam_r (name, &grp, buffer, PW_BUF_SIZE, &result) == 0)
	    && result != NULL) {
		*gidp = result->gr_gid;
		return 0;
	}

	/*
	 *  If group name was not valid, perhaps it is a  valid GID.
	 */
	errno = 0;
	l = strtol (name, &p, 10);
	if (((errno == ERANGE) && ((l == LONG_MIN) || (l == LONG_MAX)))
	   || (name == p)
	   || (*p != '\0')
	   || (l < 0)
	   || (l > INT_MAX))
		return -1;

	/*
	 *  Now ensure the supplied uid is in the user database
	 */
	if ((_getgrgid_r (l, &grp, buffer, PW_BUF_SIZE, &result) != 0)
	    || result == NULL)
		return -1;

	*gidp = (gid_t) l;
	return 0;
}

char *
gid_to_string (gid_t gid)
{
	struct group grp, *result;
	char buffer[PW_BUF_SIZE], *gstring;
	int rc;

	rc = _getgrgid_r(gid, &grp, buffer, PW_BUF_SIZE, &result);
	if (rc == 0 && result)
		gstring = slurm_xstrdup(result->gr_name);
	else
		gstring = slurm_xstrdup("nobody");
	return gstring;
}
