#include "utils.hpp"
#include <iostream>                     // for operator<<, cerr, ostream
#include "SystemException.hpp"


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
    std::cerr << "ThreadPool allocation failure\n";
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


JsonObject::JsonObject(const std::string& encodedJson)
  : m_jsonObject(json_object()) {
  decode(encodedJson);
}

/**
 * @brief JsonObject::~JsonObject
 */
JsonObject::~JsonObject() {
  if (m_lastArray) json_decref(m_lastArray);
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
void JsonObject::setProperty(const std::string& key, const int& value) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_integer(value))) {
    throw SystemException(ERRCODE_SYSTEM, "Can't set object property " + key);
  }
}

/**
   * @brief addArrayProperty Add an empty array property to the object
   * @param The key of the array
   */
void JsonObject::setArrayProperty(const std::string& key) {
  m_lastArray = json_array();
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), m_lastArray)) {
    throw SystemException(ERRCODE_SYSTEM, "Can't set the array propety + " + key);
  }
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
   * @return The encoded string
   */
std::string JsonObject::encode(void) {
  char* encodedJson = json_dumps(m_jsonObject, 0);
  std::string result = std::string(encodedJson);
  free(encodedJson);
  return result;
}

/**
   * @brief decode
   * @param encodedJson
   */
void JsonObject::decode(const std::string& encodedJson) {
  json_error_t error;
  m_jsonObject = json_loads(encodedJson.c_str(), 0, &error);
  if (! m_jsonObject) {
    throw SystemException(ERRCODE_SYSTEM, error.text);
  }
}

/**
   * @brief getProperty
   * @param key
   * @return
   */
json_t* JsonObject::getPropertyValue(const std::string& key) {
  json_t * jsonValue;
  if (! (jsonValue = json_object_get(m_jsonObject, key.c_str()))) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid profile received");
  }
  return jsonValue;
}

/**
   * @brief getProperty
   * @param key
   * @return
   */
int JsonObject::getIntProperty(const std::string& key) {
  return json_integer_value(getPropertyValue(key.c_str()));
}

/**
 * @brief getStringProperty
 * @param key
 * @return
 */
std::string JsonObject::getStringProperty(const std::string& key) {
  return json_string_value(getPropertyValue(key));
}


/**
 * @brief getArrayProperty
 * @param key
 * @param values
 */
void JsonObject::getArrayProperty(const std::string& key, std::vector<std::string>& values) {

  m_lastArray = getPropertyValue(key);
  json_t* jsonValue;
  for(size_t index = 0; index < json_array_size(m_lastArray) &&
      (jsonValue = json_array_get(m_lastArray, index));
      ++index) {
    values.push_back(json_string_value(jsonValue));
  }
}
