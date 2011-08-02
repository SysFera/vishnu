
#ifndef _HOSTLIST_H
#define _HOSTLIST_H

#include <unistd.h>		/* load ssize_t definition */

#include "slurm.h"

/*
 * hostlist_create():
 *
 * Create a new hostlist from a string representation.
 *
 * The string representation may contain one or more hostnames or
 * bracketed hostlists separated by either `,' or whitespace (e.g.
 * "alpha,beta,gamma"). A bracketed hostlist is denoted by a common
 * prefix followed by a list of numeric ranges contained within brackets
 * (e.g. "tux[0-5,12,20-25]"). Note that the numeric ranges can include
 * one or more leading zeros to indicate the numeric portion has a
 * fixed number of digits (e.g. "linux[0000-1023]").
 *
 * To support the BlueGene system's 3-D topology, a node name prefix
 * is followed by three digits identifying the node's position in
 * the X, Y and Z positions respectively. For example "bgl123" represents
 * the node or midplane with an X position of 1, Y of 2, and Z of 3.
 * A rectangular prism may be described using two endpoint locations
 * separated by "x" (e.g. "bgl[123x456]" selects all nodes with X
 * positions between 1 and 4 inclusive, Y between 2 and 5, and Z between
 * 3 and 6 for a total of 4*4*4=64 nodes). Two or more rectangular
 * prisms may be specified using comma separators within the brackets
 * (e.g. "bgl[000x133,400x533]").
 *
 * Note: if this module is compiled with WANT_RECKLESS_HOSTRANGE_EXPANSION
 * defined, a much more loose interpretation of host ranges is used.
 * Reckless hostrange expansion allows all of the following (in addition to
 * bracketed hostlists):
 *
 *  o tux0-5,tux12,tux20-25
 *  o tux0-tux5,tux12,tux20-tux25
 *  o tux0-5,12,20-25
 *
 * If str is NULL, an empty hostlist is created and returned.
 *
 * If the create fails, hostlist_create() returns NULL.
 *
 * The returned hostlist must be freed with hostlist_destroy()
 *
 */
hostlist_t hostlist_create(const char *hostlist);

char *hostlist_ranged_string_xmalloc(hostlist_t hl);


#endif /* !_HOSTLIST_H */
