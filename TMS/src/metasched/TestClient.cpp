//POINTS TO VISHNU
#include "zhelpers.hpp"
#include "JsonObject.hpp"
#include "metasched.h"
#include <zmq.hpp>



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
  assert (cloud.host_number >= cloud.cpu_number_hosts_le_50p);
  assert (cloud.host_number >= cloud.mem_number_hosts_le_50p);
  assert (cloud.highest_mean_resource_le_50_p >= 0);

  jo.setProperty("cloud_id", cloud.cloud_id);
  jo.setProperty("load_cpu", cloud.load_cpu);
  jo.setProperty("load_memory", cloud.load_memory);
  jo.setProperty("vm_number", cloud.vm_number);
  jo.setProperty("highest_mean_resource_le_50_p", cloud.highest_mean_resource_le_50_p);
  jo.setProperty("host_number", cloud.host_number);
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

int main() {
  zmq::context_t context(1);
  LazyPirateClient client(context, "tcp://localhost:5555", TIMEOUT);
  printf("Client started\n");

  //INIT REQUEST PROFILE
  rpc_profile_t profile;
  std::vector<std::string> v_param;
  profile.name=METASCHED_REQUEST;
  profile.param_count=0;
  //    add_param_to_profile(&profile, "RESOURCE");
  //    add_param_to_profile(&profile, "6");

  //ADD PARAMS
  metasched_task_t t;
  t.task_type = TASK_BATCH;
  t.id_cloud_owner = 2;
  t.id_cloud_comesFrom = 2;
  t.task_id = 1;
  JsonObject taskJson = create_task_info_json_object(t);

  metasched_cloud_t c;
  c.cloud_id = 4;
  c.load_cpu = 80;
  c.load_memory = 20;
  c.vm_number = 10;
  c.cpu_number_hosts_le_50p = 1;
  c.mem_number_hosts_le_50p = 3;
  c.highest_mean_resource_le_50_p = 30;
  c.host_number = 4;
  JsonObject cloudJson = create_cloud_json_object(c);

  c.cloud_id = 2;
  c.load_cpu = 20;
  c.load_memory = 30;
  c.vm_number = 4;
  c.cpu_number_hosts_le_50p = 2;
  c.mem_number_hosts_le_50p = 0;
  c.highest_mean_resource_le_50_p = 10;
  c.host_number = 2;
  JsonObject cloudJson2 = create_cloud_json_object(c);

  std::cout << "TASK JSON: "<< taskJson.encode() << std::endl;
  std::cout << "CLOUD JSON: "<< cloudJson.encode() << std::endl;
  std::cout << "CLOUD JSON 2: "<< cloudJson2.encode() << std::endl;

  /**
    *   -Task informations
    *   -N times cloud information
    */
  add_param_to_profile(&profile, taskJson.encode());
  add_param_to_profile(&profile, cloudJson.encode());
  add_param_to_profile(&profile, cloudJson2.encode());

  std::cout << "Vishnu Request: "<< JsonObject::serialize(profile) << std::endl;

  client.send(JsonObject::serialize(profile));
  std::cout << "Received Response? " << client.recv() << std::endl;
  printf("Done.\n");
}


