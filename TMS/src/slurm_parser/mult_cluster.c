
#include "mult_cluster.h"
#include "xmalloc.h"
#include "xstring.h"

#include "slurm/slurmdb.h"
#include "list.h"
#include "opt.h"

typedef struct {
	slurmdb_cluster_rec_t *cluster_rec;
	int preempt_cnt;
	time_t start_time;
} local_cluster_rec_t;

static char *local_cluster_name; /* name of local_cluster      */

void _destroy_local_cluster_rec(void *object)
{
	xfree(object);
}

static int _sort_local_cluster(local_cluster_rec_t* rec_a,
			       local_cluster_rec_t* rec_b)
{
	if (rec_a->start_time < rec_b->start_time)
		return -1;
	else if (rec_a->start_time > rec_b->start_time)
		return 1;

	if (rec_a->preempt_cnt < rec_b->preempt_cnt)
		return -1;
	else if (rec_a->preempt_cnt > rec_b->preempt_cnt)
		return 1;

	if(!strcmp(local_cluster_name, rec_a->cluster_rec->name))
		return -1;
	else if(!strcmp(local_cluster_name, rec_b->cluster_rec->name))
		return 1;

	return 0;
}

/*
 * We don't use the api here because it does things we aren't needing
 * like printing out information and not returning times.
 */
local_cluster_rec_t *_job_will_run (job_desc_msg_t *req)
{
	slurm_msg_t req_msg, resp_msg;
	will_run_response_msg_t *will_run_resp;
	int rc;
	char buf[64];
	char *type = "processors";
	local_cluster_rec_t *local_cluster = NULL;
	/* req.immediate = true;    implicit */

	slurm_msg_t_init(&req_msg);
	req_msg.msg_type = REQUEST_JOB_WILL_RUN;
	req_msg.data     = req;

	rc = slurm_send_recv_controller_msg(&req_msg, &resp_msg);

	if (rc < 0) {
		slurm_seterrno(SLURM_SOCKET_ERROR);
		return NULL;
	}
	switch (resp_msg.msg_type) {
	case RESPONSE_SLURM_RC:
		rc = ((return_code_msg_t *) resp_msg.data)->return_code;
		slurm_free_return_code_msg(resp_msg.data);
		if (rc)
			slurm_seterrno(rc);
		break;
	case RESPONSE_JOB_WILL_RUN:
		if (working_cluster_rec->flags & CLUSTER_FLAG_BG)
			type = "cnodes";
		will_run_resp = (will_run_response_msg_t *) resp_msg.data;
		slurm_make_time_str(&will_run_resp->start_time,
				    buf, sizeof(buf));
		debug("Job %u to start at %s on cluster %s using %u %s on %s",
		      will_run_resp->job_id, buf, working_cluster_rec->name,
		      will_run_resp->proc_cnt, type,
		      will_run_resp->node_list);
		local_cluster = xmalloc(sizeof(local_cluster_rec_t));
		local_cluster->cluster_rec = working_cluster_rec;
		local_cluster->start_time = will_run_resp->start_time;
		if (will_run_resp->preemptee_job_id) {
			local_cluster->preempt_cnt =
				list_count(will_run_resp->preemptee_job_id);
			if (opt.verbose >= LOG_LEVEL_DEBUG) {
				ListIterator itr;
				uint32_t *job_id_ptr;
				char *job_list = NULL, *sep = "";
				itr = list_iterator_create(will_run_resp->preemptee_job_id);
				while ((job_id_ptr = list_next(itr))) {
					if (job_list)
						sep = ",";
					xstrfmtcat(job_list, "%s%u",
						   sep, *job_id_ptr);
				}
				debug("  Preempts: %s", job_list);
				xfree(job_list);
			}
		}

		slurm_free_will_run_response_msg(will_run_resp);
		break;
	default:
		slurm_seterrno(SLURM_UNEXPECTED_MSG_ERROR);
		return NULL;
		break;
	}
	return local_cluster;
}

extern int sbatch_set_first_avail_cluster(job_desc_msg_t *req)
{
	int rc = SLURM_SUCCESS;
	ListIterator itr;
	local_cluster_rec_t *local_cluster = NULL;
	char buf[64];
	bool host_set = false;
	List ret_list = NULL;

	/* return if we only have 1 or less clusters here */
	if (!opt.clusters || !list_count(opt.clusters)) {
		return rc;
	} else if (list_count(opt.clusters) == 1) {
		working_cluster_rec = list_peek(opt.clusters);
		return rc;
	}

	if ((req->alloc_node == NULL) &&
	    (gethostname_short(buf, sizeof(buf)) == 0)) {
		req->alloc_node = buf;
		host_set = true;
	}

	ret_list = list_create(_destroy_local_cluster_rec);
	if (ret_list == NULL)
		fatal("list_create malloc failure");
	itr = list_iterator_create(opt.clusters);
	while ((working_cluster_rec = list_next(itr))) {
		if ((local_cluster = _job_will_run(req))) {
			if (!ret_list)
				ret_list = list_create(
					   _destroy_local_cluster_rec);
			list_append(ret_list, local_cluster);
		} else
			error("Problem with submit to cluster %s: %m",
			      working_cluster_rec->name);
	}
	list_iterator_destroy(itr);

	if (host_set)
		req->alloc_node = NULL;

	if (!ret_list) {
		error("Can't run on any of the clusters given");
		return SLURM_ERROR;
	}

	/* sort the list so the first spot is on top */
	local_cluster_name = slurm_get_cluster_name();
	list_sort(ret_list, (ListCmpF)_sort_local_cluster);
	xfree(local_cluster_name);
	local_cluster = list_peek(ret_list);

	/* set up the working cluster and be done */
	working_cluster_rec = local_cluster->cluster_rec;
	list_destroy(ret_list);

	return rc;
}

