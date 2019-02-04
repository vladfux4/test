#include <iostream>
#include <memory>

#include "static_pool_alloc.h"
#include "block_pool.h"
#include "worker.h"
#include "consumer_worker.h"
#include "scheduler_worker.h"
#include "generator_worker.h"
#include "compute_worker.h"

#include <boost/make_unique.hpp>

#include <glog/logging.h>

#include <boost/ptr_container/ptr_vector.hpp>

void* operator new(std::size_t n)  {
  auto ret_val = malloc(n);
  return ret_val;
}

void operator delete(void* p) {
  return free(p);
}

class Controller {
 public:
  Controller()
      : block_pool_(8),
        scheduler_(generators_, computers_),
        computers_(),
        generators_() {
    for (std::size_t i = 0; i < 2; ++i) {
      computers_.push_back(new ComputeWorker(scheduler_));
    }

    for (std::size_t i = 0; i < 2; ++i) {
      generators_.push_back(new GeneratorWorker(scheduler_, block_pool_));
    }
  }

  void Start() {
    LOG(INFO) << "Start Scheduler";
    scheduler_.Start();

    for (std::size_t i = 0; i < computers_.size(); ++i) {
      LOG(INFO) << "Start Computers: " << i;
      computers_[i].Start();
    }

    for (std::size_t i = 0; i < generators_.size(); ++i) {
      LOG(INFO) << "Start Generator: " << i;
      generators_[i].Start();
    }
  }

  void Join() {
    for (std::size_t i = 0; i < generators_.size(); ++i) {
      generators_[i].Join();
    }

    for (std::size_t i = 0; i < computers_.size(); ++i) {
      computers_[i].Join();
    }

    scheduler_.Join();
  }

 private:
  BlockPool block_pool_;
  SchedulerWorker scheduler_;
  boost::ptr_vector<ComputeWorker> computers_;
  boost::ptr_vector<GeneratorWorker> generators_;
};

int main(int argc, char* argv[]) {
  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = 1;
  FLAGS_log_prefix = 1;
  FLAGS_v = 1;
  google::InitGoogleLogging(argv[0]);

  Controller ctrl;
  ctrl.Start();
  ctrl.Join();

  LOG(INFO) << "Done";

  return 0;
}
