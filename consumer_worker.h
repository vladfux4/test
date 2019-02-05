#ifndef CONSUMER_WORKER_H_
#define CONSUMER_WORKER_H_

#include "worker.h"
#include <queue>
#include <boost/thread.hpp>
#include <boost/pool/pool_alloc.hpp>

class WorkerData : public Worker {
 public:
  WorkerData()
      : notify_(false),
        event_mutex_(),
        cond_var_(),
        done_(false) {}

  bool IsEventReady() {
    return notify_;
  }

  bool notify_;
  boost::mutex event_mutex_;
  boost::condition_variable cond_var_;
  bool done_;
};

template <class... Events>
class TupleWorkerBase : public WorkerData {
 public:

  void NotifyStopListen() {
    boost::unique_lock<boost::mutex> lock(event_mutex_);
    notify_ = true;
    StopListen();
    cond_var_.notify_one();
  }

  void StopListen() {
    done_ = true;
  }

  void PushEvent() {}
  void LoopEvents() {}
  std::size_t GetEventsCount() { return 0; }
};

template <class Event, class... Events>
class TupleWorkerBase<Event, Events...> : public TupleWorkerBase<Events...> {
 public:
  TupleWorkerBase()
      : TupleWorkerBase<Events...>(),
        events_() {}

  virtual void TakeEvent(Event event) = 0;

  using TupleWorkerBase<Events...>::PushEvent;
  void PushEvent(const Event event) {
    boost::unique_lock<boost::mutex> lock(WorkerData::event_mutex_);
    events_.push(event);
    WorkerData::notify_ = true;
    WorkerData::cond_var_.notify_one();
  }

  void LoopEvents() {
    while (false == events_.empty()) {
      auto event = events_.front();
      TakeEvent(event);
      events_.pop();
    }

    TupleWorkerBase<Events...>::LoopEvents();
  }

  std::size_t GetEventsCount() {
    return (events_.size() + TupleWorkerBase<Events...>::GetEventsCount());
  }

  std::queue<Event, std::deque<Event, boost::fast_pool_allocator<Event>>> events_;
};

template <class... Events>
class ConsumerWorker : public TupleWorkerBase<Events...> {
 public:
  ConsumerWorker() : TupleWorkerBase<Events...>() {}

  virtual ~ConsumerWorker() {
  }

  virtual void ProcessEvents() = 0;

  virtual void Routine() override {
    boost::unique_lock<boost::mutex> lock(WorkerData::event_mutex_);

    while ((false == WorkerData::done_) ||
           (0 != TupleWorkerBase<Events...>::GetEventsCount())) {
      WorkerData::cond_var_.wait(lock, boost::bind(&WorkerData::IsEventReady, this));
      TupleWorkerBase<Events...>::LoopEvents();
      WorkerData::notify_ = false;

      lock.unlock();
      ProcessEvents();
      lock.lock();
    }
  }
};

#endif // CONSUMER_WORKER_H_
