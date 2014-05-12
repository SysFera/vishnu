#include "metasched.hpp"
#include "utils.hpp"

/**
 * @brief Creates a JSON object containing a cloud' infos
 * @param cloud struct containing infos on the cloud.
 */
JsonObject
create_cloud_json_object(const metasched_cloud_t& cloud) {
  JsonObject data;
  data.setProperty("cloud_id", cloud.cloud_id);
  data.setProperty("load_cpu", cloud.load_cpu);
  data.setProperty("load_memory", cloud.load_memory);
  data.setProperty("vm_number", cloud.vm_number);
  data.setProperty("highest_mean_resource_le_50_p", cloud.highest_mean_resource_le_50_p);
  data.setProperty("host_number", cloud.host_number);
  data.setProperty("host_ready", cloud.host_ready);
  data.setProperty("cpu_number_hosts_le_50p", cloud.cpu_number_hosts_le_50p);
  data.setProperty("mem_number_hosts_le_50p", cloud.mem_number_hosts_le_50p);
  return data;
}

/**
 * @brief Creates a JSON object containing task infos
 * @param task struct containing infos on the task to schedule
 */
JsonObject
create_task_info_json_object(const metasched_task_t& task) {
  JsonObject data;
  data.setProperty("task_type", task.task_type);
  data.setProperty("task_id", task.task_id);
  data.setProperty("id_cloud_owner", task.id_cloud_owner);
  data.setProperty("id_cloud_comesFrom", task.id_cloud_comesFrom);
  return data;
}

/**
 * @brief Creates a metasched_cloud_t object via a json object
 * @param json The json object
 */
metasched_cloud_t
json_to_cloud(JsonObject* json) {
  metasched_cloud_t c;
  if (json) {
    c.cloud_id = json->getIntProperty("cloud_id");
    c.load_cpu = json->getIntProperty("load_cpu");
    c.load_memory = json->getIntProperty("load_memory");
    c.cpu_number_hosts_le_50p = json->getIntProperty("cpu_number_hosts_le_50p");
    c.mem_number_hosts_le_50p = json->getIntProperty("mem_number_hosts_le_50p");
    c.vm_number = json->getIntProperty("vm_number");
    c.host_number = json->getIntProperty("host_number");
    c.host_ready = json->getIntProperty("host_ready");
    c.highest_mean_resource_le_50_p = json->getIntProperty("highest_mean_resource_le_50_p");
  }
  return c;
}


/**
* @brief Checks if the cloud id is present in the clouds vector
* @return A pair containing if the boolean has been found, and if it is its position in clouds
*/
std::pair<bool, int>
cloud_is_present (int cloud_id, const std::vector<metasched_cloud_t>& clouds){
  //task cloud id is present in clouds
  bool isPresent = false;
  int pos = -1;
  for (unsigned int i = 0 ; i< clouds.size() ; i++) {
    if (cloud_id == clouds.at(i).cloud_id) {
      isPresent = true;
      pos = i;
    }
  }
  std::pair<bool, int> p;
  p.first = isPresent;
  p.second = pos;
  return p;
}


/**
* \brief Find element in server data
* @param id: The task id to find
* @return The position of the data if found, -1 otherwise
*/
int
find_element_in_data(int id, std::vector<server_data_t>& data) {
  for (unsigned int i = 0 ; i < data.size() ; i++) {
    if (id == data.at(i).task_id)
      return i;
  }
  return -1;
}
/**
* @brief Choice function for the clouds
*/
int
choose_cloud(const metasched_task_t& task, std::vector<metasched_cloud_t> clouds, std::vector<server_data_t>& data) {

  int chosen_cloud = -1;

  bool cloudOwnerIsPresent = cloud_is_present(task.id_cloud_owner, clouds).first;
  //Allocate to cloud owner
  if (cloudOwnerIsPresent && task.id_cloud_owner != task.id_cloud_comesFrom) {
    return task.id_cloud_owner;
  }

  int taskIndex = find_element_in_data(task.task_id, data);
  //If first time, we add it
  if (taskIndex < 0) {
    std::cout << "New task: creating timestamp" << std::endl;
    server_data_t serverdata;
    serverdata.nb_tries = 1;
    serverdata.task_id = task.task_id;
    serverdata.timestamp = time(NULL);
    data.push_back(serverdata);
  } else {
    //send to DC
    if (data.at(taskIndex).nb_tries > SERVER_MAX_TRIES) {
      std::cout << "SENDING TO DATA CENTER" << std::endl;
    } else {
      std::cout << "Try number " << data.at(taskIndex).nb_tries +1 << std::endl;
      data.at(taskIndex).nb_tries = data.at(taskIndex).nb_tries + 1;
    }
  }

  //removes cloud comes from from list
  std::pair<bool, int> p = cloud_is_present(task.id_cloud_comesFrom, clouds);
  if (p.first) {
    clouds.erase(clouds.begin() + p.second);
  }


  std::sort(clouds.begin(), clouds.end(), ranking_clouds);
  float value = -1* std::numeric_limits<float>::infinity();
  //Computes all cloud weight
  for (unsigned int i=0 ; i< clouds.size() ; i++) {
    metasched_cloud_t c = clouds.at(i);
    float x = getX(c);
    float y = getY(c);
    int z = i;
    float newValue = x*((float)c.host_number / 2.0)
                     - y * (float)c.host_number
                     - (float)z;
    printf("%.3f*%.3f - %.3f*%.3f - %d == %f\n", x
           , (float)c.host_number / 2.0
           , y
           , (float)c.host_number
           , z
           , newValue);
    std::cout << value << " < " << newValue << std::endl;
    if (newValue >value) {
      chosen_cloud = c.cloud_id;
      value = newValue;
    }
  }

  if (chosen_cloud == -1) {
    std::cerr << "Error: Invalid choice of cloud !" << std::endl;
  }
  return chosen_cloud;
}


/**
 * @brief Prints the information of a metasched_cloud_t object
 */
void
print_cloud(const metasched_cloud_t& cloudInfo) {
  std::cout << "*** CLOUD *** " << std::endl;
  std::cout << "cloud_id = " << cloudInfo.cloud_id << std::endl;
  std::cout << "vm_number = " << cloudInfo.vm_number << std::endl;
  std::cout << "host_number = " << cloudInfo.host_number << std::endl;
  std::cout << "host_ready = " << cloudInfo.host_ready << std::endl;
  std::cout << "load_cpu = " << cloudInfo.load_cpu << std::endl;
  std::cout << "load_memory = " << cloudInfo.load_memory << std::endl;
  //    std::cout << "cpu_number_hosts_gt_50p = " << c.cpu_number_hosts_gt_50p << std::endl;
  std::cout << "cpu_number_hosts_le_50p = " << cloudInfo.cpu_number_hosts_le_50p << std::endl;
  //    std::cout << "mem_number_hosts_gt_50p = " << c.mem_number_hosts_gt_50p << std::endl;
  std::cout << "mem_number_hosts_le_50p = " << cloudInfo.mem_number_hosts_le_50p << std::endl;
  std::cout << "highest_mean_resource_le_50_p = " << cloudInfo.highest_mean_resource_le_50_p << std::endl;
}


/**
* \brief Mean Load of hosts
*/
float
getX(const metasched_cloud_t& cloud) {
  return std::max((float)cloud.cpu_number_hosts_le_50p/(float)cloud.host_number, (float)cloud.mem_number_hosts_le_50p/(float)cloud.host_number);
}

/**
* \brief Mean size of least loaded hosts
*/
float
getY(const metasched_cloud_t& cloud) {
  return (float)(cloud.highest_mean_resource_le_50_p / 100.0);
}

/**
*   @brief Used to rank clouds by mean size of VM for the highest loaded resource
*/
bool
ranking_clouds(const metasched_cloud_t& cloud1, const metasched_cloud_t& cloud2) {
  float z1 = std::max((float)cloud1.load_cpu / (float)cloud1.vm_number
                      , (float)cloud1.load_memory/(float)cloud1.vm_number);
  float z2 = std::max((float)cloud2.load_cpu / (float)cloud2.vm_number
                      , (float)cloud2.load_memory/(float)cloud2.vm_number);
  return (z1 <= z2);
}
