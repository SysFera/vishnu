#include "metasched.hpp"

/**
 * @brief Creates a JSON object containing a cloud' infos
 * @param cloud struct containing infos on the cloud.
 */
JsonObject create_cloud_json_object(metasched_cloud_t cloud) {
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
JsonObject create_task_info_json_object(metasched_task_t task) {
  JsonObject data;
  data.setProperty("task_type", task.task_type);
  data.setProperty("task_id", task.task_id);
  data.setProperty("id_cloud_owner", task.id_cloud_owner);
  data.setProperty("id_cloud_comesFrom", task.id_cloud_comesFrom);
  return data;
}
