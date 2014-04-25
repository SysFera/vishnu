#include "metasched.hpp"

/**
 * @brief Creates a JSON object containing a cloud' infos
 * @param cloud struct containing infos on the cloud.
 */
JsonObject create_cloud_json_object(metasched_cloud_t cloud) {
  JsonObject jo;
  assert (cloud.cloud_id > 0);
  assert (cloud.load_cpu >= 0);
  assert (cloud.load_memory >= 0);
  assert (cloud.vm_number >= 0);
  assert (cloud.host_number > 0);
  assert (cloud.host_number >= cloud.host_ready);
  assert (cloud.host_number >= cloud.cpu_number_hosts_le_50p);
  assert (cloud.host_number >= cloud.mem_number_hosts_le_50p);
  assert (cloud.highest_mean_resource_le_50_p >= 0);

  jo.setProperty("cloud_id", cloud.cloud_id);
  jo.setProperty("load_cpu", cloud.load_cpu);
  jo.setProperty("load_memory", cloud.load_memory);
  jo.setProperty("vm_number", cloud.vm_number);
  jo.setProperty("highest_mean_resource_le_50_p", cloud.highest_mean_resource_le_50_p);
  jo.setProperty("host_number", cloud.host_number);
  jo.setProperty("host_ready", cloud.host_ready);
  jo.setProperty("cpu_number_hosts_le_50p", cloud.cpu_number_hosts_le_50p);
  jo.setProperty("mem_number_hosts_le_50p", cloud.mem_number_hosts_le_50p);
  return jo;
}

/**
 * @brief Creates a JSON object containing task infos
 * @param task struct containing infos on the task to schedule
 */
JsonObject create_task_info_json_object(metasched_task_t task) {
  JsonObject jo;
  assert(task.task_type > 0);
  assert(task.id_cloud_owner > 0);
  assert(task.task_id > 0);

  jo.setProperty("task_type", task.task_type);
  jo.setProperty("task_id", task.task_id);
  jo.setProperty("id_cloud_owner", task.id_cloud_owner);
  jo.setProperty("id_cloud_comesFrom", task.id_cloud_comesFrom);
  return jo;
}
