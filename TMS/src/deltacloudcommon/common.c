/*
 * Brief: wait that instance boots
 *
 */
#include <stdio.h>
#include "common.h"

const int REQUEST_RETRY_INTERVAL = 5;

int wait_for_instance_boot(struct deltacloud_api *api, const char *instid,
		struct deltacloud_instance *instance)
{
  const MAX_TIMEOUT=60;
  int timeout = MAX_TIMEOUT;
	int found = 1;

	/* wait for up to 5 minutes for the instance to show up */
	while (1) {
    fprintf(stderr, "Waiting for instance to go RUNNING, timeout: %d/%d\n", timeout, MAX_TIMEOUT);

		if (deltacloud_get_instance_by_id(api, instid, instance) < 0) {
			break;
		}

		/* if the instance went to running, we are done */
		if (strcmp(instance->state, "RUNNING") == 0) {
			found = 0;
			break;
		} else if (strcmp(instance->state, "PENDING") != 0) {
			break;
		} else {
      timeout-=REQUEST_RETRY_INTERVAL;
      if (timeout <= 0) {
				break;
			}
			deltacloud_free_instance(instance);
		}

		sleep(REQUEST_RETRY_INTERVAL);
	}

	return found;
}
