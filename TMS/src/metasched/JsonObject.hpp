/**
 * \file communication/utils.hpp
 * \brief this file contains a declaration of helper functions used by the communication layer
 * \authors Kevin Coulomb (kevin.coulomb@sysfera.com)
 */

#ifndef _JSON_OBJECT_HPP_
#define _JSON_OBJECT_HPP_

#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <jansson.h>


typedef struct rpc_profile_t {
  /**
   * \brief Overload of DIET param, last IN param in the array
   */
  int param_count;
  /**
   * \brief Overload of DIET param, name of the service
   */
  std::string name;
  /**
   * \brief Overload of DIET param
   */
  std::vector<std::string> params;
} rpc_profile_t;



class JsonObject {

public:

  static const int UNDEFINED_PROPERTY = -1;

  /**
   * @brief Constructors
   */
  JsonObject(void);
  JsonObject(const std::string& data);

  /**
   * @brief Destructor
   */
  ~JsonObject();

  void
  reset(const std::string& data);

  /**
   * @brief addProperty Add a string property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
  void
  setProperty(const std::string& key, const std::string& value);

  /**
   * @brief addProperty Add an integer property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
  void
  setProperty(const std::string& key, int value);

  /**
   * @brief addArrayProperty Add an empty array property to the object
   * @param The key of the array
   */
  void
  setArrayProperty(const std::string& key);

  /**
   * @brief addItemToLastArray Add an item to the last array added
   * @param value To value of the item
   */
  void addItemToLastArray(const std::string& value);

  /**
   * @brief encodedString Get the encoded string corresponding to the json object
   *\param flag The flag for encoding
   * @return The encoded string
   */
  std::string
  encode(int flag=0);

  /**
   * @brief decode
   * @param encodedJson
   */
  void
  decode(const std::string& encodedJson);


  /**
   * @brief getPropertyValue
   * @param key
   * @return
   */
  json_t*
  getRequiredProperty(const std::string& key);


  /**
   * @brief getIntProperty
   * @param key
   * @param defaultValue
   * @return
   */
  int
  getIntProperty(const std::string& key, int defaultValue = UNDEFINED_PROPERTY);

  /**
   * @brief getStringProperty
   * @param key
   * @return
   */
  std::string
  getStringProperty(const std::string& key);


  /**
   * @brief getArrayProperty
   * @param key
   * @param values
   */
  void
  getArrayProperty(const std::string& key, std::vector<std::string>& values);


  /**
   * @brief serialize
   * @param prof
   * @param flag
   * @return
   */
  static std::string
  serialize(const rpc_profile_t& prof, int flag=0);


  /**
   * \brief To deserialize a profile
   * \param prof The serialized profile
   * \return The deserialized profile
 */
  static rpc_profile_t
  my_deserialize(const std::string& encoedProfile);

private:
  /**
      * @brief m_json
      */
  json_t* m_jsonObject;

  /**
    * @brief m_lastArray
    */
  json_t* m_lastArray;
};


#endif /* _JSON_OBJECT_HPP_ */
