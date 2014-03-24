#include "zhelpers.hpp"
#include "JsonObject.hpp"
#include "metasched.h"
#include <zmq.hpp>

class LazyPirateServer {
public:
  /**
   * \brief Constructor
   * \param ctx the zmq context
   * \param addr the address to bind
   */
  LazyPirateServer(zmq::context_t& ctx,
                   const std::string& addr)
    : addr_(addr), ctx_(ctx) {
    reset();
  }

  void
  serve(int flags = 0) {
    //Main loop
    while (1) {
        //Receive request
        zmq::message_t request;
        server->recv(&request, 0);

        std::string sreq = zmqString_to_stdString(&request);
        //std::cout << "Received request (" << sreq << ")" << std::endl;


        //Decode request
        rpc_profile_t profile;
        profile = JsonObject::my_deserialize(sreq);
        std::cout << "Request name: " << profile.name << " / Params number : "
                    << profile.param_count << std::endl;
        //Decode task
        JsonObject taskJson(profile.params.at(0));
        metasched_task_t task = json_to_task(taskJson);
        print_task(task);

        //Decode clouds
        std::vector<metasched_cloud_t> clouds;
        for (unsigned int i = 1 ; i < profile.params.size() ; i++) {
            JsonObject cloudJson(profile.params.at(i));
            metasched_cloud_t cloud = json_to_cloud(cloudJson);
            print_cloud(cloud);
            clouds.push_back(cloud);
        }

        //Choose cloud
        int cloud_id = choose_cloud(task, clouds);
        std::cout << "Chose cloud of ID: "<<cloud_id << std::endl;

        //Send reply
        profile.name = METASCHED_RESPONSE;
        profile.param_count = 2;
        profile.params.clear();
        JsonObject jo;
        jo.setProperty("cloud_id", cloud_id);
        if (cloud_id != -1)
            profile.params.push_back("SUCCESS");
        else
            profile.params.push_back("ERROR");

        profile.params.push_back(jo.encode());

        //std::cout << "REPLY: "<< JsonObject::serialize(profile) << std::endl;
        std::string s = JsonObject::serialize(profile);
        zmq::message_t reply (s.length());
        memcpy ((void *) reply.data (), s.c_str(), reply.size());
        server->send (reply);

        //Cleanup serverdata
        cleanup_server_data();
        sleep (1);
    }
  }


/**
*   @brief Used to rank clouds by mean size of VM for the highest loaded resource
*/
static bool ranking_clouds(metasched_cloud_t c1, metasched_cloud_t c2) {
    float z1 = std::max((float)c1.load_cpu / (float)c1.vm_number
                        , (float)c1.load_memory/(float)c1.vm_number);
    float z2 = std::max((float)c2.load_cpu / (float)c2.vm_number
                        , (float)c2.load_memory/(float)c2.vm_number);
    return (z1 <= z2);
}

private:
  /**
   * \brief Reset the connection
   */
  void
  reset() {
    server.reset(new zmq::socket_t(ctx_, ZMQ_REP));
    server->bind(addr_.c_str());
    data.clear();
  }

  std::string zmqString_to_stdString(zmq::message_t *msg) {
    //Data zmqString to str
    char* decData;
    int decDataLength = msg->size();
    decData = static_cast<char*>(msg->data());
    const char* pos = decData + decDataLength;
    std::string ret = std::string(static_cast<const char*>(decData), pos);
    ret.erase(std::remove(ret.begin(), ret.end(), '\0'), ret.end());
    return ret;
  }

  metasched_task_t json_to_task(JsonObject json) {
    metasched_task_t t;
    t.task_type = json.getIntProperty("task_type");
    t.id_cloud_owner = json.getIntProperty("id_cloud_owner");
    t.id_cloud_comesFrom = json.getIntProperty("id_cloud_comesFrom");
    t.task_id = json.getIntProperty("task_id");
    return t;
}

metasched_cloud_t json_to_cloud(JsonObject json) {
    metasched_cloud_t c;
    c.cloud_id = json.getIntProperty("cloud_id");
    c.load_cpu = json.getIntProperty("load_cpu");
    c.load_memory = json.getIntProperty("load_memory");
    c.cpu_number_hosts_le_50p = json.getIntProperty("cpu_number_hosts_le_50p");
    c.mem_number_hosts_le_50p = json.getIntProperty("mem_number_hosts_le_50p");
    c.vm_number = json.getIntProperty("vm_number");
    c.host_number = json.getIntProperty("host_number");
    c.highest_mean_resource_le_50_p = json.getIntProperty("highest_mean_resource_le_50_p");
    return c;
}

void print_task(metasched_task_t t) {
    std::cout << "*** TASK *** " << std::endl;
    std::cout << "task_id = " << t.task_id << std::endl;
    std::cout << "task type = " << t.task_type << std::endl;
    std::cout << "cloud_id_owner = " << t.id_cloud_owner << std::endl;
    std::cout << "cloud_id_comes_from = " << t.id_cloud_comesFrom << std::endl;
}

void print_cloud(metasched_cloud_t c) {
    std::cout << "*** CLOUD *** " << std::endl;
    std::cout << "cloud_id = " << c.cloud_id << std::endl;
    std::cout << "vm_number = " << c.vm_number << std::endl;
    std::cout << "host_number = " << c.host_number << std::endl;
    std::cout << "load_cpu = " << c.load_cpu << std::endl;
    std::cout << "load_memory = " << c.load_memory << std::endl;
//    std::cout << "cpu_number_hosts_gt_50p = " << c.cpu_number_hosts_gt_50p << std::endl;
    std::cout << "cpu_number_hosts_le_50p = " << c.cpu_number_hosts_le_50p << std::endl;
//    std::cout << "mem_number_hosts_gt_50p = " << c.mem_number_hosts_gt_50p << std::endl;
    std::cout << "mem_number_hosts_le_50p = " << c.mem_number_hosts_le_50p << std::endl;
    std::cout << "highest_mean_resource_le_50_p = " << c.highest_mean_resource_le_50_p << std::endl;
}

/**
* @brief Checks if the cloud id is present in the clouds vector
* @return A pair containing if the boolean has been found, and if it is its position in clouds
*/
std::pair<bool, int> cloud_is_present (int cloud_id, std::vector<metasched_cloud_t> clouds){
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
* @brief Choice function for the clouds
*/
int choose_cloud(metasched_task_t task, std::vector<metasched_cloud_t> clouds) {

    int chosen_cloud = -1;


    bool cloudOwnerIsPresent = cloud_is_present(task.id_cloud_owner, clouds).first;
    //Allocate to cloud owner
    if (cloudOwnerIsPresent && task.id_cloud_owner != task.id_cloud_comesFrom) {
        return task.id_cloud_owner;
    }

    int i = find_element_in_data(task.task_id);
    //If first time, we add it
    if (i < 0) {
        std::cout << "New task: creating timestamp" << std::endl;
        server_data_t serverdata;
        serverdata.nb_tries=1;
        serverdata.task_id=task.task_id;
        serverdata.timestamp=time(NULL);
        data.push_back(serverdata);
    }
    else {
        //send to DC
        if (data.at(i).nb_tries > SERVER_MAX_TRIES) {
            std::cout << "SENDING TO DATA CENTER" << std::endl;
        }
        else {
            std::cout << "Try number " << data.at(i).nb_tries +1 << std::endl;
            data.at(i).nb_tries = data.at(i).nb_tries + 1;
        }
    }

    //removes cloud comes from from list
    std::pair<bool, int> p = cloud_is_present(task.id_cloud_comesFrom, clouds);
    if (p.first) {
        clouds.erase(clouds.begin() + p.second);
    }


    std::sort(clouds.begin(), clouds.end(), LazyPirateServer::ranking_clouds);
    float value = -INFINITY;
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
  * \brief Mean Load of hosts
  */
  float getX(metasched_cloud_t c){
    return std::max((float)c.cpu_number_hosts_le_50p/(float)c.host_number, (float)c.mem_number_hosts_le_50p/(float)c.host_number);
  }

  /**
  * \brief Mean size of least loaded hosts
  */
  float getY(metasched_cloud_t c){
    return (float)(c.highest_mean_resource_le_50_p / 100.0);
  }

  /**
  * \brief Find element in server data
  * @param id: The task id to find
  * @return The position of the data if found, -1 otherwise
  */
  int find_element_in_data(int id) {
    for (unsigned int i = 0 ; i < data.size() ; i++) {
        if (id == data.at(i).task_id)
            return i;
    }
    return -1;
  }

  void cleanup_server_data() {
    time_t now = time(NULL);
    for (unsigned int i = 0 ; i<data.size() ; i++) {
        if (data.at(i).timestamp + TIMEOUT_SERVERDATA < now) {
            std::cout << "Deleting timestamp for task " << data.at(i).task_id << std::endl;
            data.erase(data.begin()+i);
        }
    }
  }

  /**
   * \brief The address
   */
  std::string addr_;
  /**
   * \brief The context
   */
  zmq::context_t& ctx_;
  /**
   * \brief server socket
   */
  boost::scoped_ptr<zmq::socket_t> server;
  /**
  * \brief
  */
  std::vector<server_data_t> data;
};


int main() {
    std::cout << "Server Begin." << std::endl;
    zmq::context_t context(1);
    LazyPirateServer lps(context, "tcp://*:5555");
    lps.serve();

    return 0;

}

