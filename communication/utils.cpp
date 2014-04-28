#include "utils.hpp"
#include <iostream>
#include <sys/wait.h>
#include "SystemException.hpp"
#include "TMS_Data/Job.hpp"
#include "TMS_Data/SubmitOptions.hpp"
#include "TMS_Data/CancelOptions.hpp"
#include "TMS_Data/JobOutputOptions.hpp"
#include "TMS_Data/LoadCriterion.hpp"
#include "UMS_Data/Session.hpp"
#include "UserException.hpp"
#include "utilVishnu.hpp"
#include "Logger.hpp"

ThreadPool::ThreadPool()
  : nb_(boost::thread::hardware_concurrency() * 2), finished_(false) {
  setup();
}

ThreadPool::ThreadPool(int nb) : nb_(nb), finished_(false) {
  setup();
}

ThreadPool::~ThreadPool() {
  finished_ = true;
  workers_.join_all();
}


void
ThreadPool::setup() {
  try {
    for (int i = 0; i < nb_; ++i) {
      workers_.add_thread(
            new boost::thread(&ThreadPool::WorkerThread, this));
    }
  } catch (boost::thread_resource_error& e) {
    finished_ = true;
    LOG("ThreadPool allocation failure", LogInfo);
  }
}

void
ThreadPool::WorkerThread() {
  // as long as our pool is running, we process tasks
  while(!finished_) {
    Task task;
    tasks_.wait_and_pop(task);
    task();
  }
}


/**
   * @brief JsonObject::JsonObject
   */
JsonObject::JsonObject(void)
  : m_jsonObject(json_object()) {
}


JsonObject::JsonObject(const std::string& data)
  : m_jsonObject(json_object()) {
  decode(data);
}

void JsonObject::reset(const std::string& data) {
  json_decref(m_jsonObject);
  m_jsonObject = json_object();
  decode(data);
}

JsonObject::JsonObject(const TMS_Data::SubmitOptions& submitOptions)
  : m_jsonObject(json_object()) {
  setProperty("name", submitOptions.getName());
  setProperty("queue", submitOptions.getQueue());
  setProperty("walltime", submitOptions.getWallTime());
  setProperty("memory", submitOptions.getMemory());
  setProperty("nbcpu", submitOptions.getNbCpu());
  setProperty("nbnodesandcpupernode", submitOptions.getNbNodesAndCpuPerNode());
  setProperty("outputpath", submitOptions.getOutputPath());
  setProperty("errorpath", submitOptions.getErrorPath());
  setProperty("mailnotification", submitOptions.getMailNotification());
  setProperty("mailnotifyuser", submitOptions.getMailNotifyUser());
  setProperty("group", submitOptions.getGroup());
  setProperty("workingdir", submitOptions.getWorkingDir());
  setProperty("cputime", submitOptions.getCpuTime());
  setProperty("selectqueueautom", submitOptions.isSelectQueueAutom());
  setProperty("fileparams", submitOptions.getFileParams());
  setProperty("textparams", submitOptions.getTextParams());
  setProperty("workid", submitOptions.getWorkId());
  setProperty("specificparams", submitOptions.getSpecificParams());
  setProperty("posix", submitOptions.isPosix());
  setProperty("machine", submitOptions.getMachine());
  setProperty("criterion",
              const_cast<TMS_Data::SubmitOptions&>(submitOptions).getCriterion()->getLoadType());
}



JsonObject::JsonObject(const UMS_Data::Session& sessionInfo)
  : m_jsonObject(json_object()) {

  setProperty("sessionid", sessionInfo.getSessionId());
  setProperty("userid", sessionInfo.getUserId());
  setProperty("sessionkey", sessionInfo.getSessionKey());
  setProperty("lastconnect", sessionInfo.getDateLastConnect());
  setProperty("firstconnect", sessionInfo.getDateCreation());
  setProperty("expire", sessionInfo.getDateClosure());
  setProperty("status", sessionInfo.getStatus());
  setProperty("closepolicy", sessionInfo.getClosePolicy());
  setProperty("timeout", sessionInfo.getTimeout());
  setProperty("authenid", sessionInfo.getAuthenId());
}

JsonObject::JsonObject(const TMS_Data::CancelOptions& options)
  : m_jsonObject(json_object()) {

  setProperty("machineid", options.getMachineId());
  setProperty("user", options.getUser());
  setProperty("jobid", options.getJobId());
}

JsonObject::JsonObject(const TMS_Data::JobOutputOptions& options)
  : m_jsonObject(json_object()) {

  setProperty("machineid", options.getMachineId());
  setProperty("outputdir", options.getOutputDir());
  setProperty("days", options.getDays());
}



/**
 * @brief JsonObject::~JsonObject
 */
JsonObject::~JsonObject() {
  json_decref(m_jsonObject);
}

/**
   * @brief addProperty Add a string property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
void JsonObject::setProperty(const std::string& key, const std::string& value) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_string(value.c_str()))) {
    throw SystemException(ERRCODE_SYSTEM, "Can't set object property " + key);
  }
}

/**
   * @brief addProperty Add an integer property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
void JsonObject::setProperty(const std::string& key, int value) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_integer(value))) {
    throw SystemException(ERRCODE_SYSTEM, "Can't set object property " + key);
  }
}

/**
   * @brief addArrayProperty Add an empty array property to the object
   * @param The key of the array
   */
void JsonObject::setArrayProperty(const std::string& key) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_array())) {
    throw SystemException(ERRCODE_SYSTEM, "Can't set the array propety + " + key);
  }
  m_lastArray = getRequiredProperty(key.c_str());
}

/**
   * @brief addItemToLastArray Add an item to the last array added
   * @param value To value of the item
   */
void JsonObject::addItemToLastArray(const std::string& value) {
  if (0 != json_array_append_new(m_lastArray, json_string(value.c_str()))) {
    throw SystemException(ERRCODE_SYSTEM, "Can't add item into json array");
  }
}

/**
   * @brief encodedString Get the encoded string corresponding to the json object
   * \param flag The flag for encoding
   * @return The encoded string
   */
std::string JsonObject::encode(int flag) {
  char* encodedJson = json_dumps(m_jsonObject, flag);
  std::string result = std::string(encodedJson);
  free(encodedJson);
  return result;
}

/**
   * @brief decode
   * @param encodedJson
   */
void JsonObject::decode(const std::string& data) {
  json_error_t error;
  m_jsonObject = json_loads(data.c_str(), 0, &error);
  if (! m_jsonObject) {
    throw SystemException(ERRCODE_INVDATA,
                          boost::str(boost::format("error when parsing invalid json data [%1%]") % data));
  }
}

/**
   * @brief getProperty
   * @param key
   * @return
   */
json_t* JsonObject::getRequiredProperty(const std::string& key) {
  json_t * jsonValue;
  if (! (jsonValue = json_object_get(m_jsonObject, key.c_str()))) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid profile received");
  }
  return jsonValue;
}


/**
 * @brief getIntProperty
 * @param key
 * @param defaultValue
 * @return
 */
int JsonObject::getIntProperty(const std::string& key, int defaultValue) {
  json_t * jsonValue = json_object_get(m_jsonObject, key.c_str());
  return jsonValue? json_integer_value(jsonValue) : defaultValue;
}

/**
 * @brief getStringProperty
 * @param key
 * @return
 */
std::string JsonObject::getStringProperty(const std::string& key) {
  json_t* jsonValue = json_object_get(m_jsonObject, key.c_str());
  std::string res;
  if (jsonValue) {
    const char* paramValue = json_string_value(jsonValue);
    if (paramValue) {
      res.assign(paramValue);
    } else {
      throw SystemException(ERRCODE_INVDATA,
                            boost::str(boost::format("null or invalid property %1%") % key));
    }
  } else {
    throw SystemException(ERRCODE_INVDATA,
                          boost::str(boost::format("null or invalid property %1%") % key));
  }
  return res;
}


/**
 * @brief getArrayProperty
 * @param key
 * @param values values must be strings or it will crash
 */
void JsonObject::getArrayProperty(const std::string& key, std::vector<std::string>& values) {

  m_lastArray = getRequiredProperty(key);
  json_t* jsonValue;
  for(size_t index = 0; index < json_array_size(m_lastArray); ++index) {
    if (jsonValue = json_array_get(m_lastArray, index)) {
      const char* paramValue = json_string_value(jsonValue);
      if (paramValue) {
        values.push_back(paramValue);
      } else {
        throw SystemException(ERRCODE_INVDATA,
                              boost::str(boost::format("Array contains null or "
                                                       "invalid string data at index %1%") % index));
      }
    } else {
      throw SystemException(ERRCODE_INVDATA,
                            boost::str(boost::format("Array contains null or "
                                                     "invalid string data at index %1%") % index));
    }
  }
}


/**
 * @brief JsonObject::serialize
 * @param prof
 * @param flag
 * @return
 */
std::string
JsonObject::serialize(diet_profile_t* prof, int flag) {

  if (!prof) {
    throw SystemException(ERRCODE_SYSTEM, "Cannot serialize a null pointer profile");
  }

  JsonObject jsonProfile;
  jsonProfile.setProperty("name", prof->name);
  jsonProfile.setProperty("param_count", prof->param_count);

  // Set params
  jsonProfile.setArrayProperty("params");
  for (int i = 0; i< prof->param_count; ++i) {
    jsonProfile.addItemToLastArray(prof->params[i]);
  }
  return jsonProfile.encode(flag);
}

/**
 * @brief JsonObject::serialize
 * @param job
 * @param flag
 * @return
 */
std::string
JsonObject::serialize(const TMS_Data::Job& job, int flag) {

  JsonObject jsonProfile;
  jsonProfile.setProperty("vsession", job.getSessionId());
  jsonProfile.setProperty("submitmachineid", job.getSubmitMachineId());
  jsonProfile.setProperty("submitmachinename", job.getSubmitMachineName());
  jsonProfile.setProperty("jobid", job.getJobId());
  jsonProfile.setProperty("workid", job.getWorkId());
  jsonProfile.setProperty("jobname", job.getJobName());
  jsonProfile.setProperty("batchjobid", job.getBatchJobId());
  jsonProfile.setProperty("jobpath", job.getJobPath());
  jsonProfile.setProperty("outputpath", job.getOutputPath());
  jsonProfile.setProperty("errorpath", job.getErrorPath());
  jsonProfile.setProperty("outputdir", job.getOutputDir());
  jsonProfile.setProperty("jobprio", job.getJobPrio());
  jsonProfile.setProperty("nbcpus", job.getNbCpus());
  jsonProfile.setProperty("jobworkingdir", job.getJobWorkingDir());
  jsonProfile.setProperty("status", job.getStatus());
  jsonProfile.setProperty("submitdate", job.getSubmitDate());
  jsonProfile.setProperty("enddate", job.getEndDate());
  jsonProfile.setProperty("owner", job.getOwner());
  jsonProfile.setProperty("jobqueue", job.getJobQueue());
  jsonProfile.setProperty("wallclocklimit", job.getWallClockLimit());
  jsonProfile.setProperty("groupname", job.getGroupName());
  jsonProfile.setProperty("jobdescription", job.getJobDescription());
  jsonProfile.setProperty("memlimit", job.getMemLimit());
  jsonProfile.setProperty("nbnodes", job.getNbNodes());
  jsonProfile.setProperty("nbnodesandcpupernode", job.getNbNodesAndCpuPerNode());
  jsonProfile.setProperty("userid", job.getUserId());
  jsonProfile.setProperty("vmip", job.getVmIp());
  jsonProfile.setProperty("vmid", job.getVmId());

  return jsonProfile.encode(flag);
}

/**
 * @brief JsonObject::deserialize
 * @param encodedJson
 * @return
 */
boost::shared_ptr<diet_profile_t>
JsonObject::deserialize(const std::string& encodedJson) {

  if (encodedJson.empty()) {
    throw SystemException(ERRCODE_SYSTEM, "Cannot deserialize an empty string");
  }

  boost::shared_ptr<diet_profile_t> profile;
  profile.reset(new diet_profile_t);

  JsonObject jsonObject(encodedJson);

  profile->name = jsonObject.getStringProperty("name");
  profile->param_count = jsonObject.getIntProperty("param_count", 0);
  jsonObject.getArrayProperty("params", profile->params);
  if (profile->params.size() != profile->param_count) {
    throw SystemException(ERRCODE_INVDATA,
                          "Incoherent profile, wrong number of parameters");
  }
  return profile;
}


/**
 * @brief getJob
 * @return
 */
TMS_Data::Job JsonObject::getJob() {

  TMS_Data::Job job;
  job.setSessionId(getStringProperty("vsession"));
  job.setSubmitMachineId(getStringProperty("submitmachineid"));
  job.setSubmitMachineName(getStringProperty("submitmachinename"));
  job.setJobId(getStringProperty("jobid"));
  job.setJobName(getStringProperty("jobname"));
  job.setBatchJobId(getStringProperty("batchjobid"));
  job.setJobPath(getStringProperty("jobpath"));
  job.setOutputPath(getStringProperty("outputpath"));
  job.setErrorPath(getStringProperty("errorpath"));
  job.setOutputDir(getStringProperty("outputdir"));
  job.setJobPrio(getIntProperty("jobprio"));
  job.setNbCpus(getIntProperty("nbcpus"));
  job.setJobWorkingDir(getStringProperty("jobworkingdir"));
  job.setStatus(getIntProperty("status"));
  job.setSubmitDate(getIntProperty("submitdate"));
  job.setEndDate(getIntProperty("enddate"));
  job.setOwner(getStringProperty("owner"));
  job.setJobQueue(getStringProperty("jobqueue"));
  job.setWallClockLimit(getIntProperty("wallclocklimit"));
  job.setGroupName(getStringProperty("groupname"));
  job.setJobDescription(getStringProperty("jobdescription"));
  job.setMemLimit(getIntProperty("memlimit"));
  job.setNbNodes(getIntProperty("nbnodes"));
  job.setUserId(getStringProperty("userid"));
  job.setNbNodesAndCpuPerNode(getStringProperty("nbnodesandcpupernode"));
  job.setVmIp(getStringProperty("vmip"));
  job.setVmId(getStringProperty("vmid"));

  return job;
}


/**
 * @brief getSubmitOptions
 * @return
 */
TMS_Data::SubmitOptions JsonObject::getSubmitOptions() {

  TMS_Data::SubmitOptions options;
  options.setName(getStringProperty("name"));
  options.setQueue(getStringProperty("queue"));
  options.setWallTime(getIntProperty("walltime"));
  options.setMemory(getIntProperty("memory"));
  options.setNbCpu(getIntProperty("nbcpu"));
  options.setNbNodesAndCpuPerNode(getStringProperty("nbnodesandcpupernode"));
  options.setOutputPath(getStringProperty("outputpath"));
  options.setErrorPath(getStringProperty("errorpath"));
  options.setMailNotification(getStringProperty("mailnotification"));
  options.setMailNotifyUser(getStringProperty("mailnotifyuser"));
  options.setGroup(getStringProperty("group"));
  options.setWorkingDir(getStringProperty("workingdir"));
  options.setCpuTime(getStringProperty("cputime"));
  options.setSelectQueueAutom(getIntProperty("selectqueueautom"));
  options.setFileParams(getStringProperty("fileparams"));
  options.setTextParams(getStringProperty("textparams"));
  options.setWorkId(getIntProperty("workid"));
  options.setSpecificParams(getStringProperty("specificparams"));
  options.setPosix(getIntProperty("posix"));
  options.setMachine(getStringProperty("machine"));

  TMS_Data::LoadCriterion_ptr criterion = new TMS_Data::LoadCriterion();
  criterion->setLoadType(getIntProperty("criterion", 1));
  options.setCriterion(criterion);

  return options;
}


/**
 * @brief Get port number from a given uri
 * @param uri : the uri address
 * @return the port number, throw exception on error
 */
int
vishnu::getPortFromUri(const std::string& uri) {

  size_t pos = uri.rfind(":");
  int port = -1;
  if (pos == std::string::npos ||
      (port = vishnu::convertToInt(uri.substr(pos+1, std::string::npos))) <= 0) {
    throw UserException(ERRCODE_INVALID_PARAM, "The format of the uri is invalid or the uri contains unallowed characters");
  }

  return port;
}

/**
 * @brief getHostFromUrl
 * @param uri
 * @return
 */
std::string
vishnu::getHostFromUri(const std::string& uri) {

  std::string host;
  size_t pos1 = uri.find("://");
  size_t pos2 = uri.rfind(":");
  // Extract the host in the uri
  if (pos1 != std::string::npos && pos2 != std::string::npos) {
    size_t pos = pos1+3;
    size_t len = pos2 - pos;
    host = uri.substr(pos, len);
  }
  // Check that the address is valid
  if (host.empty() || host.find_first_of("*") != std::string::npos) {
    throw UserException(ERRCODE_INVALID_PARAM,
                        "The format of the uri is invalid or the uri contains unallowed characters");
  }
  return host;
}

/**
 * \brief Function to validate an URI
 * \throws a VishnuException if contains the '*'
 * \param uri the uri to check, throw exception on error
 */
void
vishnu::validateUri(const std::string & uri)
{
  LOG(boost::str(boost::format("[INFO] Parsing the uri (%1%)...")% uri), LogInfo);
  vishnu::getPortFromUri(uri);
  vishnu::getHostFromUri(uri);
}

/**
 * @brief Exit a process if a given is different to zero
 * @param code The code
 */
void
vishnu::exitProcessOnError(int code)
{
  if (code != 0) {
    exit(code);
  }
}

/**
 * @brief Exit a process if its child failed
 * @param child The pid of the child process
 */
void
vishnu::exitProcessOnChildError(pid_t child)
{
  int retCode;
  waitpid(child, &retCode, 0);
  if (! WIFEXITED(retCode) || WEXITSTATUS(retCode) != 0) {
    kill(-1, SIGKILL);
    exit(retCode);
  }
}
