#ifndef METASCHED_H
#define METASCHED_H


#include <ctime>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <vector>

class JsonObject;
#define SERVER_MAX_TRIES 3

/**
 * Contains infos on the task
 * task_type:           The type of task
 * id_cloud_owner:      The cloud id of the owner of the task, aka where the request originated from
 * id_cloud_comesFrom:  The id of the cloud where the task comes from (if its not a first scheduling)
 * If it is first scheduling, set it to NEW_TASK
 */
struct metasched_task_t {
    int task_type;
    int id_cloud_owner;
    int id_cloud_comesFrom;
    int task_id;
};


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
struct metasched_cloud_t {
    int cloud_id;
    int load_cpu;
    int load_memory;
    int vm_number;
    int host_number;
    int host_ready;
    int cpu_number_hosts_le_50p;
    int mem_number_hosts_le_50p;
    int highest_mean_resource_le_50_p;
};

/**
* Data used by the server
* task_id:      The id of the task
* timestamp:    The last time data for this task was updated
* nb_tries:     The number of times we already handled this task
*/
struct server_data_t {
    int task_id;
    time_t timestamp;
    int nb_tries;
};

/**
 * @brief Creates a JSON object containing a cloud' infos
 * @param cloud struct containing infos on the cloud.
 */
JsonObject
create_cloud_json_object(const metasched_cloud_t& cloud);

/**
 * @brief Creates a JSON object containing task infos
 * @param task struct containing infos on the task to schedule
 */
JsonObject
create_task_info_json_object(const metasched_task_t& task);

/**
 * @brief Creates a metasched_cloud_t object via a json object
 * @param json The json object
 */
metasched_cloud_t
json_to_cloud(JsonObject* json);

/**
 * @brief Prints the information of a metasched_cloud_t object
 */
void
print_cloud(const metasched_cloud_t& cloudInfo);

/**
* @brief Checks if the cloud id is present in the clouds vector
* @return A pair containing if the boolean has been found, and if it is its position in clouds
*/
std::pair<bool, int>
cloud_is_present (int cloud_id, const std::vector<metasched_cloud_t>& clouds);


/**
* \brief Find element in server data
* @param id: The task id to find
* @return The position of the data if found, -1 otherwise
*/
int
find_element_in_data(int id, std::vector<server_data_t>& data);

/**
* @brief Choice function for the clouds
*/
int
choose_cloud(const metasched_task_t& task, std::vector<metasched_cloud_t> clouds, std::vector<server_data_t>& data);

/**
* \brief Mean Load of hosts
*/
float
getX(const metasched_cloud_t& cloud);

/**
* \brief Mean size of least loaded hosts
*/
float
getY(const metasched_cloud_t& cloud);

/**
*   @brief Used to rank clouds by mean size of VM for the highest loaded resource
*/
bool
ranking_clouds(const metasched_cloud_t& cloud1, const metasched_cloud_t& cloud2);

#endif /* METASCHED_H */

