#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <functional>
#include <queue>
#include <vector>
#include <boost/thread.hpp>
#include <boost/version.hpp>

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
  bool finished_; /**< is our pool running or not ? */
};




#endif /* _UTILS_HPP_ */
