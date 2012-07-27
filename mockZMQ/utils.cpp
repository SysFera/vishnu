#include "utils.hpp"


ThreadPool::ThreadPool()
  : finished_(false), nb_(boost::thread::hardware_concurrency() * 2) {
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
