/**
 * \file communication/utils.hpp
 * \brief this file contains a declaration of helper functions used by the communication layer
 * \authors Kevin Coulomb (kevin.coulomb@sysfera.com)
 */

#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <functional>
#include <queue>
#include <vector>
#include <boost/thread.hpp>
#include <boost/version.hpp>
#include <jansson.h>
#include "DIET_client.h"


class diet_profile_t;
namespace TMS_Data {
  class Session;
  class SubmitOptions;
  class Job;
  class CancelOptions;
}

namespace UMS_Data {
  class Session;
}

/**
 * \brief Definition of a task
 */
typedef boost::function0<void> Task;

namespace threadsafe {

  /**
 * @class queue
 * @brief simple thread-safe queue
 */
  template<typename T>
  class queue {
  public:
    queue() {};

    void
    push(T value) {
      boost::lock_guard<boost::mutex> lock(mutex_);
      {
        data_.push(value);
      }

      cond_.notify_one();
    }

    bool
    try_pop(T& value) {
      boost::lock_guard<boost::mutex> lock(mutex_);
      if (data_.empty()) {
        return false;
      }
      value = data_.front();
      data_.pop();
    }

    void
    wait_and_pop(T& value) {
      // we use unique_lock since we'll use it with conditions
      boost::unique_lock<boost::mutex> lock(mutex_);
      while (data_.empty()) {
        cond_.wait(lock);
      }
      value = data_.front();
      data_.pop();
    }

    bool
    empty() const {
      boost::lock_guard<boost::mutex> lock(mutex_);
      return data_.empty();
    }

  private:
    mutable boost::mutex mutex_; /**< mutex protecting access */
    boost::condition_variable cond_; /**< condition checking that we have data ready */
    std::queue<T> data_; /**< data storage */
  };

} /* namespace threadsafe */


/**
 * @class ThreadPool
 * @brief simple pool of threads
 */
class ThreadPool {
public:
  /**
   * @brief default constructor
   * sets number of threads to CPU cores * 2
   */
  ThreadPool();
  /**
   * @brief constructor
   * @param nb number of threads
   */
  ThreadPool(int nb);
  /**
   * @brief destructor
   */
  ~ThreadPool();

  /**
   * \brief add a function to a task
   * \param f function to call
   */
  template<typename Callable>
  void
  submit(Callable f) {
    tasks_.push(f);
  }


private:
  /**
   * @brief common initialization to all constructors
   */
  void
  setup();

  /**
   * @brief thread unit body
   */
  void
  WorkerThread();

  /* Note: boost::threads are not copyable so we can't store them in a
     standard container so we use boost::thread_group instead to manage
     our actual threads. */
  boost::thread_group workers_;/**< worker threads */
  threadsafe::queue<Task> tasks_; /**< scheduled tasks */
  int nb_; /**< number of allocated threads */
  volatile bool finished_; /**< is our pool running or not ? */
};



class JsonObject {

public:

  static const int UNDEFINED_PROPERTY = -1;

  /**
   * @brief Constructors
   */
  JsonObject(void);
  JsonObject(const std::string& data);
  JsonObject(const TMS_Data::SubmitOptions& submitOptions);
  JsonObject(const UMS_Data::Session& sessionInfo);
  JsonObject(const TMS_Data::CancelOptions& options);

  /**
   * @brief Destructor
   */
  ~JsonObject();

  void reset(const std::string& data);

  /**
   * @brief addProperty Add a string property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
  void setProperty(const std::string& key, const std::string& value);

  /**
   * @brief addProperty Add an integer property to the object
   * @param key The key of the property
   * @param value The value of the key
   */
  void setProperty(const std::string& key, int value);

  /**
   * @brief addArrayProperty Add an empty array property to the object
   * @param The key of the array
   */
  void setArrayProperty(const std::string& key);

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
  std::string encode(int flag=0);

  /**
   * @brief decode
   * @param encodedJson
   */
  void decode(const std::string& encodedJson);


  /**
   * @brief getPropertyValue
   * @param key
   * @return
   */
  json_t* getRequiredProperty(const std::string& key);


  /**
   * @brief getIntProperty
   * @param key
   * @param defaultValue
   * @return
   */
  int getIntProperty(const std::string& key, int defaultValue = UNDEFINED_PROPERTY);

  /**
   * @brief getStringProperty
   * @param key
   * @return
   */
  std::string getStringProperty(const std::string& key);


  /**
   * @brief getArrayProperty
   * @param key
   * @param values
   */
  void getArrayProperty(const std::string& key, std::vector<std::string>& values);



  /**
   * @brief serialize
   * @param prof
   * @param flag
   * @return
   */
  static std::string
  serialize(diet_profile_t* prof, int flag=0);


  /**
   * @brief serialize
   * @param job
   * @param flag
   * @return
   */
  static std::string
  serialize(const TMS_Data::Job& job, int flag=0);

  /**
   * @brief deserialize
   * @param encodedJson
   * @return
   */
  static boost::shared_ptr<diet_profile_t>
  deserialize(const std::string& encodedJson);

  /**
   * @brief getJob
   * @return
   */
  TMS_Data::Job getJob();

  /**
   * @brief getSubmitOptions
   * @return
   */
  TMS_Data::SubmitOptions getSubmitOptions();

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


#endif /* _UTILS_HPP_ */
