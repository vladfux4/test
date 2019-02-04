#ifndef WORKER_H_
#define WORKER_H_

#include <boost/thread.hpp>

class Worker {
 public:
  Worker() : thread_() {
  }

  virtual ~Worker() {
  }

  void Start() {
    thread_ = boost::thread(boost::bind(&Worker::Routine, this));
  }

  virtual void Routine() = 0;

  void Join() {
    thread_.join();
  }

 protected:
  boost::thread thread_;
};

#endif // WORKER_H_
