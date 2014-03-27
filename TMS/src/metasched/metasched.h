#define TIMEOUT 10
//timeout for server data to expire (in seconds)
#define TIMEOUT_SERVERDATA 10

#define TASK_BATCH 1
#define TASK_INTERACTIVE 2
#define TASK_SERVICE 3

#define METASCHED_REQUEST "metasched_request"
#define METASCHED_RESPONSE "metasched_response"

#define NEW_TASK 0

//Maximum tries we do before sending to data center
#define SERVER_MAX_TRIES 3

#include "JsonObject.hpp"
#include <ctime>
#include <cassert>
#include <boost/lexical_cast.hpp>


/**
 * Contains infos on the task
 * task_type:           The type of task
 * id_cloud_owner:      The cloud id of the owner of the task, aka where the request originated from
 * id_cloud_comesFrom:  The id of the cloud where the task comes from (if its not a first scheduling)
 * If it is first scheduling, set it to NEW_TASK
 */
typedef struct metasched_task_t {
    int task_type;
    int id_cloud_owner;
    int id_cloud_comesFrom;
    int task_id;
} metasched_task_t;


/**
 * Contains infos on cloud
 * cloud_id:                    The id of the cloud
 * load_cpu:                    Global cpu load
 * load_memory:                 Global MEM load
 * vm_number:                   Number of Virtual Machines
 * host_number:                 Number of Physical hosts
 * host_number:                 Number of Physical hosts ready to run virtual machines
 * cpu_number_host_le_50p:      Number of hosts that are loaded at less than or equal 50% (CPU)
 * mem_number_hosts_gt_50p:     Number of hosts that are loaded at less than or equal 50% (MEM)
 * highest_mean_resource_le_50_p:   Highest mean load for hosts that are < 50% (Highest between CPU or MEM)
 */
typedef struct metasched_cloud_t {
    int cloud_id;
    int load_cpu;
    int load_memory;
    int vm_number;
    int host_number;
    int host_ready;
    int cpu_number_hosts_le_50p;
    int mem_number_hosts_le_50p;
    int highest_mean_resource_le_50_p;
} metasched_cloud_t;

/**
* Data used by the server
* task_id:      The id of the task
* timestamp:    The last time data for this task was updated
* nb_tries:     The number of times we already handled this task
*/
typedef struct server_data_t {
    int task_id;
    time_t timestamp;
    int nb_tries;

} server_data_t;

/**
 * @brief Adds a String parameter to the profile
 * @param profile The profile
 * @param s The parameter to add represented as a string
 */
void add_param_to_profile(rpc_profile_t * profile, std::string s) {
    assert (profile != NULL);
    profile->params.push_back(s);
    profile->param_count += 1;
    assert ((unsigned)profile->params.size() == (unsigned)profile->param_count);
}
/**
 * @brief Adds a Int parameter to the profile
 * @param profile The profile
 * @param i The parameter to add represented as a string
 */
void add_param_to_profile(rpc_profile_t * profile, int i) {
    assert (profile != NULL);
    profile->params.push_back(boost::lexical_cast<std::string>(i));
    profile->param_count += 1;
    assert ((unsigned)profile->params.size() == (unsigned)profile->param_count);
}


