#include "JsonObject.hpp"

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
//    throw SystemException(ERRCODE_SYSTEM, "Can't set object property " + key);
    std::cerr << "Can't set object property " << key;
  }
}

/**
   * @brief addProperty Add an integer property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
void JsonObject::setProperty(const std::string& key, int value) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_integer(value))) {
//    throw SystemException(ERRCODE_SYSTEM, "Can't set object property " + key);
    std::cerr << "Can't set object property " << key;
  }
}

/**
   * @brief addArrayProperty Add an empty array property to the object
   * @param The key of the array
   */
void JsonObject::setArrayProperty(const std::string& key) {
  if (0 != json_object_set_new(m_jsonObject, key.c_str(), json_array())) {
//    throw SystemException(ERRCODE_SYSTEM, "Can't set the array propety + " + key);
    std::cerr << "Can't set the array propety + "<< key;
  }
  m_lastArray = getRequiredProperty(key.c_str());
}

/**
   * @brief addItemToLastArray Add an item to the last array added
   * @param value To value of the item
   */
void JsonObject::addItemToLastArray(const std::string& value) {
  if (0 != json_array_append_new(m_lastArray, json_string(value.c_str()))) {
//    throw SystemException(ERRCODE_SYSTEM, "Can't add item into json array");
    std::cerr << "Can't add item into json array";
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
void JsonObject::decode(const std::string& encodedJson) {
  json_error_t error;
  m_jsonObject = json_loads(encodedJson.c_str(), 0, &error);
  if (! m_jsonObject) {
//    throw SystemException(ERRCODE_SYSTEM, error.text);
    std::cerr << error.text;
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
//    throw SystemException(ERRCODE_SYSTEM, "Invalid profile received");
    std::cerr << "Invalid profile received";
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
  return jsonValue? json_string_value(jsonValue) : "";
}


/**
 * @brief getArrayProperty
 * @param key
 * @param values
 */
void JsonObject::getArrayProperty(const std::string& key, std::vector<std::string>& values) {

  m_lastArray = getRequiredProperty(key);
  json_t* jsonValue;
  for(size_t index = 0; index < json_array_size(m_lastArray) &&
      (jsonValue = json_array_get(m_lastArray, index));
      ++index) {
    values.push_back(json_string_value(jsonValue));
  }
}


/**
 * @brief JsonObject::serialize
 * @param prof
 * @param flag
 * @return
 */
std::string
JsonObject::serialize(const rpc_profile_t& prof, int flag) {

  JsonObject jsonProfile;
  jsonProfile.setProperty("name", prof.name);
  jsonProfile.setProperty("param_count", prof.param_count);

  // Set params
  jsonProfile.setArrayProperty("params");
  for (int i = 0; i< prof.param_count; ++i) {
    jsonProfile.addItemToLastArray(prof.params[i]);
  }
  return jsonProfile.encode(flag);
}



/**
 * @brief JsonObject::deserialize
 * @param encodedJson
 * @return
 */
rpc_profile_t
JsonObject::my_deserialize(const std::string& encodedJson) {

  if (encodedJson.empty()) {
    throw "Cannot deserialize an empty string";
  }

  JsonObject jsonObject(encodedJson);
  rpc_profile_t profile;
  profile.name = jsonObject.getStringProperty("name");
  profile.param_count = jsonObject.getIntProperty("param_count");
  jsonObject.getArrayProperty("params", profile.params);

  if ((unsigned)profile.params.size() != (unsigned)profile.param_count) {
    throw "Invalid profile";
  }
  return profile;
}
