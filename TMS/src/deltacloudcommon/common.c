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
	int timeout = 300;
	int found = 0;

	/* wait for up to 5 minutes for the instance to show up */
	while (1) {
		fprintf(stderr, "Waiting for instance to go RUNNING, %d/300\n", timeout);

		if (deltacloud_get_instance_by_id(api, instid, instance) < 0) {
			/* FIXME: yuck.  If we got here, then we created an instance, but
			 * we couldn't look it up.  That means we can't destroy it either.
			 */
			fprintf(stderr, "Failed to lookup instance by id: %s\n",
					deltacloud_get_last_error_string());
			break;
		}

		/* if the instance went to running, we are done */
		if (strcmp(instance->state, "RUNNING") == 0) {
			found = 1;
			break;
		} else if (strcmp(instance->state, "PENDING") != 0) {
			fprintf(stderr, "Instance gone to a unexpected state %s, failing\n", instance->state);
			//deltacloud_instance_destroy(api, instance);
			break;
		} else {
			/* otherwise, see if we exceeded the timeout.  If so, destroy
			 * the instance and fail
			 */
			timeout--;
			if (timeout == 0) {
				fprintf(stderr, "Instance never went running, failing\n");
				//deltacloud_instance_destroy(api, instance);
				break;
			}
			deltacloud_free_instance(instance);
		}

		sleep(REQUEST_RETRY_INTERVAL);
	}

	return found;
}
