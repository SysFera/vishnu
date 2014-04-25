#ifndef METASCHED_H
#define METASCHED_H


#define TIMEOUT_SERVERDATA 10 //timeout for server data to expire (in seconds)
#define TASK_BATCH 1
#define TASK_INTERACTIVE 2
#define TASK_SERVICE 3
#define METASCHED_REQUEST "metasched_request"
#define METASCHED_RESPONSE "metasched_response"
#define NEW_TASK 0
#define SERVER_MAX_TRIES 3 // Maximum tries we do before sending to data center


#include "utils.hpp"
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
 * @brief Creates a JSON object containing a cloud' infos
 * @param cloud struct containing infos on the cloud.
 */
JsonObject create_cloud_json_object(metasched_cloud_t cloud);

/**
 * @brief Creates a JSON object containing task infos
 * @param task struct containing infos on the task to schedule
 */
JsonObject create_task_info_json_object(metasched_task_t task);

#endif /* METASCHED_H */

