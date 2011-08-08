
#ifndef _VISHNU_SLURM_UTIL_H
#define _VISHNU_SLURM_UTIL_H

#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/types.h>

#include "slurm/slurm.h"
#include "slurm/slurmdb.h"
//#include "xassert.h"
#include "xmalloc.h"
#include "xstring.h"
#include "slurm_internal_api.h"


////////////////////////////////SLURM slurmdb_defs////////////////////////
//slurmdb_cluster_rec_t *working_cluster_rec = NULL;
//#define ENV_BUFSIZE (256 * 1024)

////////////////////////////////SLURM PROC ARG UTILITY FUNCTION///////////////////////////
/*
 * get either 1 or 2 integers for a resource count in the form of either
 * (count, min-max, or '*')
 * A partial error message is passed in via the 'what' param.
 * IN arg - argument
 * IN what - variable name (for errors)
 * OUT min - first number
 * OUT max - maximum value if specified, NULL if don't care
 * IN isFatal - if set, exit on error
 * RET true if valid
 */
bool get_resource_arg_range2(const char *arg, const char *what, int* min,
          int *max, bool isFatal) 
{

  char *p;
  long int result;

  /* wildcard meaning every possible value in range */
  if ((*arg == '\0') || (*arg == '*' )) {
    *min = 1;
    if (max)
      *max = INT_MAX;
    return true;
  }

  result = strtol(arg, &p, 10);
        if (*p == 'k' || *p == 'K') {
    result *= 1024;
    p++;
  } else if(*p == 'm' || *p == 'M') {
    result *= 1048576;
    p++;
  }

  if (((*p != '\0') && (*p != '-')) || (result <= 0L)) {
    error ("Invalid numeric value \"%s\" for %s.", arg, what);
    if (isFatal)
      exit(1);
    return false;
  } else if (result > INT_MAX) {
    error ("Numeric argument (%ld) to big for %s.", result, what);
    if (isFatal)
      exit(1);
    return false;
  }

  *min = (int) result;

  if (*p == '\0')
    return true;
  if (*p == '-')
    p++;
  result = strtol(p, &p, 10);
        if ((*p == 'k') || (*p == 'K')) {
    result *= 1024;
    p++;
  } else if(*p == 'm' || *p == 'M') {
    result *= 1048576;
    p++;
  }

  if (((*p != '\0') && (*p != '-')) || (result <= 0L)) {
    error ("Invalid numeric value \"%s\" for %s.", arg, what);
    if (isFatal)
      exit(1);
    return false;
  } else if (result > INT_MAX) {
    error ("Numeric argument (%ld) to big for %s.", result, what);
    if (isFatal)
      exit(1);
    return false;
  }

  if (max)
    *max = (int) result;

  return true;

}

/*
 * verify that a resource counts in arg are of a known form X, X:X, X:X:X, or
 * X:X:X:X, where X is defined as either (count, min-max, or '*')
 * RET true if valid
 */
bool verify_socket_core_thread_count2(const char *arg, int *min_sockets,
    int *min_cores, int *min_threads,
    cpu_bind_type_t *cpu_bind_type)
{

  bool tmp_val,ret_val;
  int i,j;
  int max_sockets, max_cores, max_threads;
  const char *cur_ptr = arg;
  char buf[3][48]; /* each can hold INT64_MAX - INT64_MAX */
  buf[0][0] = '\0';
  buf[1][0] = '\0';
  buf[2][0] = '\0';

  for (j=0;j<3;j++) {
    for (i=0;i<47;i++) {
      if (*cur_ptr == '\0' || *cur_ptr ==':')
        break;
      buf[j][i] = *cur_ptr++;
    }
    if (*cur_ptr == '\0')
      break;
    //xassert(*cur_ptr == ':');
    buf[j][i] = '\0';
    cur_ptr++;
  }

  /* if cpu_bind_type doesn't already have a auto preference, choose
   * the level based on the level of the -E specification
   */
  if (!(*cpu_bind_type & (CPU_BIND_TO_SOCKETS |
          CPU_BIND_TO_CORES |
          CPU_BIND_TO_THREADS))) {
    if (j == 0) {
      *cpu_bind_type |= CPU_BIND_TO_SOCKETS;
    } else if (j == 1) {
      *cpu_bind_type |= CPU_BIND_TO_CORES;
    } else if (j == 2) {
      *cpu_bind_type |= CPU_BIND_TO_THREADS;
    }
  }
  buf[j][i] = '\0';

  ret_val = true;
  tmp_val = get_resource_arg_range2(&buf[0][0], "first arg of -B",
      min_sockets, &max_sockets, true);
  if ((*min_sockets == 1) && (max_sockets == INT_MAX))
    *min_sockets = NO_VAL;  /* Use full range of values */
  ret_val = ret_val && tmp_val;

  tmp_val = get_resource_arg_range2(&buf[1][0], "second arg of -B",
      min_cores, &max_cores, true);
  if ((*min_cores == 1) && (max_cores == INT_MAX))
    *min_cores = NO_VAL;  /* Use full range of values */
  ret_val = ret_val && tmp_val;

  tmp_val = get_resource_arg_range2(&buf[2][0], "third arg of -B",
      min_threads, &max_threads, true);
  if ((*min_threads == 1) && (max_threads == INT_MAX))
    *min_threads = NO_VAL;  /* Use full range of values */
  ret_val = ret_val && tmp_val;

  return ret_val;
}


/*
 * verify that a distribution type in arg is of a known form
 * returns the task_dist_states, or -1 if state is unknown
 */
task_dist_states_t verify_dist_type2(const char *arg, uint32_t *plane_size)
{

  int len = strlen(arg);
  char *dist_str = NULL;
  task_dist_states_t result = SLURM_DIST_UNKNOWN;
  bool lllp_dist = false, plane_dist = false;

  dist_str = strchr(arg,':');
  if (dist_str != NULL) {
    /* -m cyclic|block:cyclic|block */
    lllp_dist = true;
  } else {
    /* -m plane=<plane_size> */
    dist_str = strchr(arg,'=');
    if(dist_str != NULL) {
      *plane_size=atoi(dist_str+1);
      len = dist_str-arg;
      plane_dist = true;
    }
  }

  if (lllp_dist) {
    if (strcasecmp(arg, "cyclic:cyclic") == 0) {
      result = SLURM_DIST_CYCLIC_CYCLIC;
    } else if (strcasecmp(arg, "cyclic:block") == 0) {
      result = SLURM_DIST_CYCLIC_BLOCK;
    } else if (strcasecmp(arg, "block:block") == 0) {
      result = SLURM_DIST_BLOCK_BLOCK;
    } else if (strcasecmp(arg, "block:cyclic") == 0) {
      result = SLURM_DIST_BLOCK_CYCLIC;
    }
  } else if (plane_dist) {
    if (strncasecmp(arg, "plane", len) == 0) {
      result = SLURM_DIST_PLANE;
    }
  } else {
    if (strncasecmp(arg, "cyclic", len) == 0) {
      result = SLURM_DIST_CYCLIC;
    } else if (strncasecmp(arg, "block", len) == 0) {
      result = SLURM_DIST_BLOCK;
    } else if ((strncasecmp(arg, "arbitrary", len) == 0) ||
        (strncasecmp(arg, "hostfile", len) == 0)) {
      result = SLURM_DIST_ARBITRARY;
    }
  }

  return result;
}

/* Convert a string into a node count */
static int
_str_to_nodes2(const char *num_str, char **leftover)
{
  long int num;
  char *endptr;

  num = strtol(num_str, &endptr, 10);
  if (endptr == num_str) { /* no valid digits */
    *leftover = (char *)num_str;
    return 0;
  }
  if (*endptr != '\0' && (*endptr == 'k' || *endptr == 'K')) {
    num *= 1024;
    endptr++;
  }
  *leftover = endptr;

  return (int)num;
}

/*
 * verify that a node count in arg is of a known form (count or min-max)
 * OUT min, max specified minimum and maximum node counts
 * RET true if valid
 */
bool verify_node_count2(const char *arg, int *min_nodes, int *max_nodes)
{
  char *ptr, *min_str, *max_str;
  char *leftover;

  /* Does the string contain a "-" character?  If so, treat as a range.
   * otherwise treat as an absolute node count. */
  if ((ptr = index(arg, '-')) != NULL) {
    min_str = slurm_xstrndup(arg, ptr-arg);
    *min_nodes = _str_to_nodes2(min_str, &leftover);
    if (!slurm_xstring_is_whitespace(leftover)) {
      error("\"%s\" is not a valid node count", min_str);
      xfree(min_str);
      return false;
    }
    xfree(min_str);
    if (*min_nodes == 0)
      *min_nodes = 1;

    max_str = slurm_xstrndup(ptr+1, strlen(arg)-((ptr+1)-arg));
    *max_nodes = _str_to_nodes2(max_str, &leftover);
    if (!slurm_xstring_is_whitespace(leftover)) {
      error("\"%s\" is not a valid node count", max_str);
      xfree(max_str);
      return false;
    }
    xfree(max_str);
  } else {
    *min_nodes = *max_nodes = _str_to_nodes2(arg, &leftover);
    if (!slurm_xstring_is_whitespace(leftover)) {
      error("\"%s\" is not a valid node count", arg);
      return false;
    }
    if (*min_nodes == 0) {
      /* whitespace does not a valid node count make */
      error("\"%s\" is not a valid node count", arg);
      return false;
    }
  }

  if ((*max_nodes != 0) && (*max_nodes < *min_nodes)) {
    error("Maximum node count %d is less than"
        " minimum node count %d",
        *max_nodes, *min_nodes);
    return false;
  }

  return true;
}

/*
 * verify that a hint is valid and convert it into the implied settings
 * RET true if valid
 */
bool verify_hint2(const char *arg, int *min_sockets, int *min_cores,
    int *min_threads, int *ntasks_per_core,
    cpu_bind_type_t *cpu_bind_type)
{

  char *buf, *p, *tok;
  if (!arg) {
    return true;
  }

  buf = slurm_xstrdup(arg);
  p = buf;
  /* change all ',' delimiters not followed by a digit to ';'  */
  /* simplifies parsing tokens while keeping map/mask together */
  while (p[0] != '\0') {
    if ((p[0] == ',') && (!isdigit(p[1])))
      p[0] = ';';
    p++;
  }

  p = buf;
  while ((tok = strsep(&p, ";"))) {
    if (strcasecmp(tok, "help") == 0) {
      fprintf(stderr,
          "Application hint options:\n"
          "    --hint=             Bind tasks according to application hints\n"
          "        compute_bound   use all cores in each socket\n"
          "        memory_bound    use only one core in each socket\n"
          "        [no]multithread [don't] use extra threads with in-core multi-threading\n"
          "        help            show this help message\n");
      return 1;
    } else if (strcasecmp(tok, "compute_bound") == 0) {
      *min_sockets = NO_VAL;
      *min_cores   = NO_VAL;
      *min_threads = 1;
      *cpu_bind_type |= CPU_BIND_TO_CORES;
    } else if (strcasecmp(tok, "memory_bound") == 0) {
      *min_cores   = 1;
      *min_threads = 1;
      *cpu_bind_type |= CPU_BIND_TO_CORES;
    } else if (strcasecmp(tok, "multithread") == 0) {
      *min_threads = NO_VAL;
      *cpu_bind_type |= CPU_BIND_TO_THREADS;
      if (*ntasks_per_core == NO_VAL)
        *ntasks_per_core = INFINITE;
    } else if (strcasecmp(tok, "nomultithread") == 0) {
      *min_threads = 1;
      *cpu_bind_type |= CPU_BIND_TO_THREADS;
    } else {
      error("unrecognized --hint argument \"%s\", "
          "see --hint=help", tok);
      xfree(buf);
      return 1;
    }
  }

  xfree(buf);
  return 0;
}

uint16_t parse_mail_type2(const char *arg)
{

  uint16_t rc;

  if (strcasecmp(arg, "BEGIN") == 0)
    rc = MAIL_JOB_BEGIN;
  else if  (strcasecmp(arg, "END") == 0)
    rc = MAIL_JOB_END;
  else if (strcasecmp(arg, "FAIL") == 0)
    rc = MAIL_JOB_FAIL;
  else if (strcasecmp(arg, "REQUEUE") == 0)
    rc = MAIL_JOB_REQUEUE;
  else if (strcasecmp(arg, "ALL") == 0)
    rc = MAIL_JOB_BEGIN |  MAIL_JOB_END |  MAIL_JOB_FAIL |
      MAIL_JOB_REQUEUE;
  else
    rc = 0;   /* failure */

  return rc;
}


/* return command name from its full path name */
char * base_name2(char* command)
{
  char *char_ptr, *name;
  int i;

  if (command == NULL)
    return NULL;

  char_ptr = strrchr(command, (int)'/');
  if (char_ptr == NULL)
    char_ptr = command;
  else
    char_ptr++;

  i = strlen(char_ptr);
  name = xmalloc(i+1);
  strcpy(name, char_ptr);
  return name;
}

/*
 * str_to_mbytes2(): verify that arg is numeric with optional "K", "M", "G"
 * or "T" at end and return the number in mega-bytes
 */
long str_to_mbytes2(const char *arg){
  long result;
  char *endptr;

  errno = 0;
  result = strtol(arg, &endptr, 10);
  if ((errno != 0) && ((result == LONG_MIN) || (result == LONG_MAX)))
    result = -1;
  else if (endptr[0] == '\0')
    ;
  else if ((endptr[0] == 'k') || (endptr[0] == 'K'))
    result = (result + 1023) / 1024;  /* round up */
  else if ((endptr[0] == 'm') || (endptr[0] == 'M'))
    ;
  else if ((endptr[0] == 'g') || (endptr[0] == 'G'))
    result *= 1024;
  else if ((endptr[0] == 't') || (endptr[0] == 'T'))
    result *= (1024 * 1024);
  else
    result = -1;

  return result;
}

/* verify a node list is valid based on the dist and task count given */
bool verify_node_list2(char **node_list_pptr, enum task_dist_states dist,
    int task_count) {

  char *nodelist = NULL;

  assert (node_list_pptr);
  assert (*node_list_pptr);

  if (strchr(*node_list_pptr, '/') == NULL)
    return true;  /* not a file name */

  /* If we are using Arbitrary grab count out of the hostfile
     using them exactly the way we read it in since we are
     saying, lay it out this way! */
  if(dist == SLURM_DIST_ARBITRARY)
    nodelist = slurm_read_hostfile(*node_list_pptr, task_count);
  else
    nodelist = slurm_read_hostfile(*node_list_pptr, NO_VAL);

  if (!nodelist)
    return false;

  xfree(*node_list_pptr);
  *node_list_pptr = slurm_xstrdup(nodelist);
  free(nodelist);

  return true;

}

static void
_freeF2(void *data)
{
  xfree(data);
}

static List
_create_path_list2(void)
{
  List l = slurm_list_create(_freeF2);
  char *path = slurm_xstrdup(getenv("PATH"));
  char *c, *lc;

  c = getenv("PATH");
  if (!c) {
    error("No PATH environment variable");
    return l;
  }
  path = slurm_xstrdup(c);
  c = lc = path;

  while (*c != '\0') {
    if (*c == ':') {
      /* nullify and push token onto list */
      *c = '\0';
      if (lc != NULL && strlen(lc) > 0)
        slurm_list_append(l, slurm_xstrdup(lc));
      lc = ++c;
    } else
      c++;
  }

  if (strlen(lc) > 0)
    slurm_list_append(l, slurm_xstrdup(lc));

  xfree(path);

  return l;
}

/* search PATH to confirm the access of the given command */
char *search_path2(char *cwd, char *cmd, bool check_current_dir,
      int access_mode)
{
  List         l        = NULL;
  ListIterator i        = NULL;
  char *path, *fullpath = NULL;

  if (  (cmd[0] == '.' || cmd[0] == '/')
      && (access(cmd, access_mode) == 0 ) ) {
    if (cmd[0] == '.')
      xstrfmtcat(fullpath, "%s/", cwd);
    xstrcat(fullpath, cmd);
    goto done;
  }

  l = _create_path_list2();
  if (l == NULL)
    return NULL;

  if (check_current_dir)
    slurm_list_prepend(l, slurm_xstrdup(cwd));

  i = slurm_list_iterator_create(l);
  while ((path = slurm_list_next(i))) {
    xstrfmtcat(fullpath, "%s/%s", path, cmd);

    if (access(fullpath, access_mode) == 0)
      goto done;

    xfree(fullpath);
    fullpath = NULL;
  }
done:
  if (l)
    slurm_list_destroy(l);
  return fullpath;
}

/* Convert a signal name to it's numeric equivalent.
 * Return -1 on failure */
int sig_name2num2(char *signal_name)
{
  char *sig_name[] = {"HUP", "INT", "QUIT", "KILL", "TERM",
    "USR1", "USR2", "CONT", NULL};
  int sig_num[] = {SIGHUP, SIGINT, SIGQUIT, SIGKILL, SIGTERM,
    SIGUSR1, SIGUSR2, SIGCONT};
  char *ptr;
  long tmp;
  int sig;
  int i;

  tmp = strtol(signal_name, &ptr, 10);
  if (ptr != signal_name) { /* found a number */
    if (slurm_xstring_is_whitespace(ptr))
      sig = (int)tmp;
    else
      return 0;
  } else {
    ptr = (char *)signal_name;
    while (isspace(*ptr))
      ptr++;
    if (strncasecmp(ptr, "SIG", 3) == 0)
      ptr += 3;
    for (i = 0; ; i++) {
      if (sig_name[i] == NULL)
        return 0;
      if (strncasecmp(ptr, sig_name[i],
            strlen(sig_name[i])) == 0) {
        /* found the signal name */
        if (!slurm_xstring_is_whitespace(ptr +
              strlen(sig_name[i])))
          return 0;
        sig = sig_num[i];
        break;
      }
    }
  }
  return sig;
}

/* Translate a signal option string "--signal=<int>[@<time>]" into
 * it's warn_signal and warn_time components.
 * RET 0 on success, -1 on failure */
int get_signal_opts2(char *optarg, uint16_t *warn_signal, uint16_t *warn_time)
{
  char *endptr;
  long num;

  if (optarg == NULL)
    return -1;

  endptr = strchr(optarg, '@');
  if (endptr)
    endptr[0] = '\0';
  num = (uint16_t) sig_name2num2(optarg);
  if (endptr)
    endptr[0] = '@';
  if ((num < 1) || (num > 0x0ffff))
    return -1;
  *warn_signal = (uint16_t) num;

  if (!endptr) {
    *warn_time = 60;
    return 0;
  }

  num = strtol(endptr+1, &endptr, 10);
  if ((num < 0) || (num > 0x0ffff))
    return -1;
  *warn_time = (uint16_t) num;
  if (endptr[0] == '\0')
    return 0;
  return -1;
}

time_t     time_now;
struct tm *time_now_tm;

typedef struct unit_names {
  char *name;
  int name_len;
  int multiplier;
} unit_names_t;
static unit_names_t un[] = {
  {"minutes", 7,  60},
  {"minute",  6,  60},
  {"hours", 5,  (60*60)},
  {"hour",  4,  (60*60)},
  {"days",  4,  (24*60*60)},
  {"day",   3,  (24*60*60)},
  {"weeks", 5,  (7*24*60*60)},
  {"week",  4,  (7*24*60*60)},
  {NULL,    0,  0}
};

/* convert time differential string into a number of seconds
 * time_str (in): string to parse
 * pos (in/out): position of parse start/end
 * delta (out): delta in seconds
 * RET: -1 on error, 0 otherwise
 */
static int _get_delta(char *time_str, int *pos, long *delta)
{
  int i, offset;
  long cnt = 0;
  int digit = 0;

  for (offset = (*pos) + 1;
       ((time_str[offset] != '\0') && (time_str[offset] != '\n'));
       offset++) {
    if (isspace(time_str[offset]))
      continue;
    for (i=0; un[i].name; i++) {
      if (!strncasecmp((time_str + offset),
           un[i].name, un[i].name_len)) {
        offset += un[i].name_len;
        cnt    *= un[i].multiplier;
        break;
      }
    }
    if (un[i].name)
      break;  /* processed unit name */
    if ((time_str[offset] >= '0') && (time_str[offset] <= '9')) {
      cnt = (cnt * 10) + (time_str[offset] - '0');
      digit++;
      continue;
    }
    goto prob;
  }

  if (!digit) /* No numbers after the '=' */
    return -1;

  *pos = offset - 1;
  *delta = cnt;
  return 0;

prob:  *pos = offset - 1;
  return -1;
}

/* convert "HH:MM[:SS] [AM|PM]" string to numeric values
 * time_str (in): string to parse
 * pos (in/out): position of parse start/end
 * hour, minute, second (out): numberic values
 * RET: -1 on error, 0 otherwise
 */
static int
_get_time(char *time_str, int *pos, int *hour, int *minute, int * second)
{
  int hr, min, sec;
  int offset = *pos;

  /* get hour */
  if ((time_str[offset] < '0') || (time_str[offset] > '9'))
    goto prob;
  hr = time_str[offset++] - '0';
  if (time_str[offset] != ':') {
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    hr = (hr * 10) + time_str[offset++] - '0';
  }
  if (hr > 23) {
    offset -= 2;
    goto prob;
  }
  if (time_str[offset] != ':')
    goto prob;
  offset++;

  /* get minute */
  if ((time_str[offset] < '0') || (time_str[offset] > '9'))
                goto prob;
  min = time_str[offset++] - '0';
  if ((time_str[offset] < '0') || (time_str[offset] > '9'))
    goto prob;
  min = (min * 10)  + time_str[offset++] - '0';
  if (min > 59) {
    offset -= 2;
    goto prob;
  }

  /* get optional second */
  if (time_str[offset] == ':') {
    offset++;
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    sec = time_str[offset++] - '0';
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    sec = (sec * 10)  + time_str[offset++] - '0';
    if (sec > 59) {
      offset -= 2;
      goto prob;
    }
  } else
    sec = 0;

  while (isspace(time_str[offset])) {
    offset++;
  }
 if (strncasecmp(time_str+offset, "pm", 2)== 0) {
    hr += 12;
    if (hr > 23) {
      if (hr == 24)
        hr = 12;
      else
        goto prob;
    }
    offset += 2;
  } else if (strncasecmp(time_str+offset, "am", 2) == 0) {
    if (hr > 11) {
      if (hr == 12)
        hr = 0;
      else
        goto prob;
    }
    offset += 2;
  }

  *pos = offset - 1;
  *hour   = hr;
  *minute = min;
  *second = sec;
  return 0;

 prob:  *pos = offset;
  return -1;
}

/* convert "MMDDYY" "MM.DD.YY" or "MM/DD/YY" string to numeric values
 * or "YYYY-MM-DD string to numeric values
* time_str (in): string to parse
 * pos (in/out): position of parse start/end
 * month, mday, year (out): numberic values
 * RET: -1 on error, 0 otherwise
 */
static int _get_date(char *time_str, int *pos, int *month, int *mday, int *year)
{
  int mon, day, yr;
  int offset = *pos;

  if(time_str[offset+4] && (time_str[offset+4] == '-')
      && time_str[offset+7] && (time_str[offset+7] == '-')) {
    /* get year */
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    yr = time_str[offset++] - '0';

    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    yr = (yr * 10) + time_str[offset++] - '0';

    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    yr = (yr * 10) + time_str[offset++] - '0';

    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    yr = (yr * 10) + time_str[offset++] - '0';

    offset++; // for the -

    /* get month */
    mon = time_str[offset++] - '0';
    if ((time_str[offset] >= '0') && (time_str[offset] <= '9'))
      mon = (mon * 10) + time_str[offset++] - '0';
    if ((mon < 1) || (mon > 12)) {
      offset -= 2;
      goto prob;
    }

    offset++; // for the -

    /* get day */
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    day = time_str[offset++] - '0';
    if ((time_str[offset] >= '0') && (time_str[offset] <= '9'))
      day = (day * 10) + time_str[offset++] - '0';
    if ((day < 1) || (day > 31)) {
      offset -= 2;
      goto prob;
    }

    *pos = offset - 1;
    *month = mon - 1; /* zero origin */
    *mday  = day;
    *year  = yr - 1900;     /* need to make it mktime
                               happy 1900 == "00" */
    return 0;
  }

  /* get month */
  mon = time_str[offset++] - '0';
  if ((time_str[offset] >= '0') && (time_str[offset] <= '9'))
    mon = (mon * 10) + time_str[offset++] - '0';
  if ((mon < 1) || (mon > 12)) {
    offset -= 2;
    goto prob;
  }
  if ((time_str[offset] == '.') || (time_str[offset] == '/'))
    offset++;

  /* get day */
  if ((time_str[offset] < '0') || (time_str[offset] > '9'))
    goto prob;
  day = time_str[offset++] - '0';
  if ((time_str[offset] >= '0') && (time_str[offset] <= '9'))
    day = (day * 10) + time_str[offset++] - '0';
  if ((day < 1) || (day > 31)) {
    offset -= 2;
    goto prob;
  }
  if ((time_str[offset] == '.') || (time_str[offset] == '/'))
    offset++;

  /* get optional year */
  if ((time_str[offset] >= '0') && (time_str[offset] <= '9')) {
    yr = time_str[offset++] - '0';
    if ((time_str[offset] < '0') || (time_str[offset] > '9'))
      goto prob;
    yr = (yr * 10) + time_str[offset++] - '0';
  } else
    yr = 0;

  *pos = offset - 1;
  *month = mon - 1; /* zero origin */
  *mday  = day;
  if (yr)
    *year  = yr + 100;  /* 1900 == "00" */
  return 0;

prob:  *pos = offset;
       return -1;
}

time_t parse_time2(char *time_str, int past)
{
  int    hour = -1, minute = -1, second = 0;
  int    month = -1, mday = -1, year = -1;
  int    pos = 0;
  struct tm res_tm;

  if (strncasecmp(time_str, "uts", 3) == 0) {
    char *last = NULL;
    long uts = strtol(time_str+3, &last, 10);
    if ((uts < 1000000) || (uts == LONG_MAX) ||
        (last == NULL) || (last[0] != '\0'))
      goto prob;
    return (time_t) uts;
  }

  time_now = time(NULL);
  time_now_tm = localtime(&time_now);

  for (pos=0; ((time_str[pos] != '\0') && (time_str[pos] != '\n'));
      pos++) {
    if (isblank(time_str[pos]) ||
        (time_str[pos] == '-') || (time_str[pos] == 'T'))
      continue;
    if (strncasecmp(time_str+pos, "today", 5) == 0) {
      month = time_now_tm->tm_mon;
      mday  = time_now_tm->tm_mday;
      year  = time_now_tm->tm_year;
      pos += 4;
      continue;
    }
    if (strncasecmp(time_str+pos, "tomorrow", 8) == 0) {
      time_t later = time_now + (24 * 60 * 60);
      struct tm *later_tm = localtime(&later);
      month = later_tm->tm_mon;
      mday  = later_tm->tm_mday;
      year  = later_tm->tm_year;
      pos += 7;
      continue;
    }
    if (strncasecmp(time_str+pos, "midnight", 8) == 0) {
      hour   = 0;
      minute = 0;
      second = 0;
      pos += 7;
      continue;
    }
    if (strncasecmp(time_str+pos, "noon", 4) == 0) {
      hour   = 12;
      minute = 0;
      second = 0;
      pos += 3;
      continue;
    }
    if (strncasecmp(time_str+pos, "teatime", 7) == 0) {
      hour   = 16;
      minute = 0;
      second = 0;
      pos += 6;
      continue;
    }
    if (strncasecmp(time_str+pos, "now", 3) == 0) {
      int i;
      long delta = 0;
      time_t later;
      struct tm *later_tm;
      for (i=(pos+3); ; i++) {
        if (time_str[i] == '+') {
          pos += i;
          if (_get_delta(time_str, &pos, &delta))
            goto prob;
          break;
        }
        if (isblank(time_str[i]))
          continue;
        if ((time_str[i] == '\0')
            || (time_str[i] == '\n')) {
          pos += (i-1);
          break;
        }
        pos += i;
        goto prob;
      }
      later    = time_now + delta;
      later_tm = localtime(&later);
      month    = later_tm->tm_mon;
      mday     = later_tm->tm_mday;
      year     = later_tm->tm_year;
      hour     = later_tm->tm_hour;
      minute   = later_tm->tm_min;
      second   = later_tm->tm_sec;
      continue;
    }
    if ((time_str[pos] < '0') || (time_str[pos] > '9'))
      /* invalid */
      goto prob;
    /* We have some numeric value to process */
    if ((time_str[pos+1] == ':') || (time_str[pos+2] == ':')) {
      /* Parse the time stamp */
      if (_get_time(time_str, &pos, &hour, &minute, &second))
        goto prob;
      continue;
    }

    if (_get_date(time_str, &pos, &month, &mday, &year))
      goto prob;
  }
  if ((hour == -1) && (month == -1))    /* nothing specified, time=0 */
    return (time_t) 0;
  else if ((hour == -1) && (month != -1)) { /* date, no time implies 00:00 */
    hour = 0;
    minute = 0;
  }
  else if ((hour != -1) && (month == -1)) {
    /* time, no date implies soonest day */
    if (past || (hour >  time_now_tm->tm_hour)
        ||  ((hour == time_now_tm->tm_hour)
          && (minute > time_now_tm->tm_min))) {
      /* today */
      month = time_now_tm->tm_mon;
      mday  = time_now_tm->tm_mday;
      year  = time_now_tm->tm_year;
    } else {/* tomorrow */
      time_t later = time_now + (24 * 60 * 60);
      struct tm *later_tm = localtime(&later);
      month = later_tm->tm_mon;
      mday  = later_tm->tm_mday;
      year  = later_tm->tm_year;
    }
  }
  if (year == -1) {
    if (past) {
      if (month > time_now_tm->tm_mon) {
        /* last year */
        year = time_now_tm->tm_year - 1;
      } else  {
        /* this year */
        year = time_now_tm->tm_year;
      }
    } else if ((month  >  time_now_tm->tm_mon)
        ||  ((month == time_now_tm->tm_mon)
          && (mday >  time_now_tm->tm_mday))
        ||  ((month == time_now_tm->tm_mon)
          && (mday == time_now_tm->tm_mday)
          && (hour >  time_now_tm->tm_hour))
        ||  ((month == time_now_tm->tm_mon)
          && (mday == time_now_tm->tm_mday)
          && (hour == time_now_tm->tm_hour)
          && (minute > time_now_tm->tm_min))) {
      /* this year */
      year = time_now_tm->tm_year;
    } else {
      /* next year */
      year = time_now_tm->tm_year + 1;
    }
  }

  /* convert the time into time_t format */
  memset(&res_tm, 0, sizeof(res_tm));
  res_tm.tm_sec   = second;
  res_tm.tm_min   = minute;
  res_tm.tm_hour  = hour;
  res_tm.tm_mday  = mday;
  res_tm.tm_mon   = month;
  res_tm.tm_year  = year;
  res_tm.tm_isdst = -1;

  /*  printf("%d/%d/%d %d:%d\n",month+1,mday,year,hour,minute); */

  return mktime(&res_tm);

prob:  fprintf(stderr, "Invalid time specification (pos=%d): %s\n", pos, time_str);
       return (time_t) 0;
}

/* Convert a string to an equivalent time value
 * input formats:
 * min
 * min:sec
 * hr:min:sec
 * days-hr:min:sec
 * days-hr
 * output:
 * minutes  (or -2 on error, INFINITE is -1 as defined in slurm.h)
 * if unlimited is the value of string)
 */
extern int time_str2mins2(char *string)
{
  int days = -1, hr = -1, min = -1, sec = -1;
  int i, tmp = 0, res = 0;

  if ((string == NULL) || (string[0] == '\0'))
    return -1;  /* invalid input */
  if ((!strcasecmp(string, "-1")) ||
      (!strcasecmp(string, "INFINITE")) ||
      (!strcasecmp(string, "UNLIMITED"))) {
    return INFINITE;
  }

  for (i=0; ; i++) {
    if ((string[i] >= '0') && (string[i] <= '9')) {
      tmp = (tmp * 10) + (string[i] - '0');
    } else if (string[i] == '-') {
      if (days != -1)
        return -2;  /* invalid input */
      days = tmp;
      tmp = 0;
    } else if ((string[i] == ':') || (string[i] == '\0')) {
      if (min == -1) {
        min = tmp;
      } else if (sec == -1) {
        sec = tmp;
      } else if (hr == -1) {
        hr = min;
        min = sec;
        sec = tmp;
      } else
        return -2;  /* invalid input */
      tmp = 0;
    } else
      return -2;    /* invalid input */

    if (string[i] == '\0')
      break;
  }

  if ((days != -1) && (hr == -1) && (min != 0)) {
    /* format was "days-hr" or "days-hr:min" */
    hr = min;
    min = sec;
    sec = 0;
  }

  if (days == -1)
    days = 0;
  if (hr == -1)
    hr = 0;
  if (min == -1)
    min = 0;
  if (sec == -1)
    sec = 0;
  res = (((days * 24) + hr) * 60) + min;
  res += (sec + 59) / 60; /* round up */
  return res;
}


/*
 * Return the number of elements in the environment `env'
 */
int envcount2 (char **env)
{
int envc = 0;
  while (env[envc] != NULL)
    envc++;
  return (envc);
}

#define ENV_BUFSIZE2 (256 * 1024)

int setenvf(char ***envp, const char *name, const char *fmt, ...)
{
  char *str = NULL, *value;
  va_list ap;
  int rc;

  value = xmalloc(ENV_BUFSIZE2);
  va_start(ap, fmt);
  vsnprintf (value, ENV_BUFSIZE2, fmt, ap);
  va_end(ap);

  if (envp && *envp) {
    if (slurm_env_array_overwrite(envp, name, value) == 1)
      rc = 0;
    else
      rc = 1;
  } else {
    xstrfmtcat(str, "%s=%s", name, value);
    rc = putenv(str);
  }
  xfree(value);
  return rc;
}

int setenvfs(const char *fmt, ...)
{
  va_list ap;
  char *buf, *bufcpy;
  int rc;

  buf = xmalloc(ENV_BUFSIZE2);
  va_start(ap, fmt);
  vsnprintf(buf, ENV_BUFSIZE2, fmt, ap);
  va_end(ap);

  bufcpy = slurm_xstrdup(buf);
  xfree(buf);
  rc = putenv(bufcpy);
  return rc;
}

#endif /* !_VISHNU_SLURM_UTIL_H */
