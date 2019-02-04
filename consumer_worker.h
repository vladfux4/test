#ifndef CONSUMER_WORKER_H_
#define CONSUMER_WORKER_H_

#include "worker.h"
#include <queue>
#include <boost/thread.hpp>

template<class Event>
class ConsumerWorker : public Worker {
 public:
  ConsumerWorker()
      : notify(false),
        done(false),
        events_() {
  }

  virtual ~ConsumerWorker() {
  }

  virtual void Routine() override {
    boost::unique_lock<boost::mutex> lock(event_mutex_);

    while ((false == done) || (false == events_.empty())) {
      cond_var_.wait(lock, boost::bind(&ConsumerWorker::IsEventReady, this));

      while ((false == events_.empty())) {
        auto event = events_.front();
        TakeEvent(event);
        events_.pop();
      }

      notify = false;
      lock.unlock();
      ProcessEvents();
      lock.lock();
    }
  }

  void PushEvent(const Event event) {
    boost::unique_lock<boost::mutex> lock(event_mutex_);
    events_.push(event);
    notify = true;
    cond_var_.notify_one();
  }

  void NotifyStopListen() {
    boost::unique_lock<boost::mutex> lock(event_mutex_);
    notify = true;
    StopListen();
    cond_var_.notify_one();
  }

  bool IsEventReady() {
    return notify;
  }

  virtual void TakeEvent(const Event event) = 0;
  virtual void ProcessEvents() = 0;

 protected:
  void StopListen() {
    done = true;
  }

 private:
  boost::mutex event_mutex_;
  boost::condition_variable cond_var_;
  bool notify;
  bool done;
  std::queue<Event> events_;
};

#endif // CONSUMER_WORKER_H_
