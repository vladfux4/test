#include <glog/logging.h>
#include "controller.h"

Controller::Controller(const std::size_t block_size,
                       const std::size_t blocks_count,
                       const std::size_t generators_count,
                       const std::size_t computers_count)
    : block_pool_(block_size),
      scheduler_(generators_, computers_, blocks_count),
      computers_(),
      generators_() {
  for (std::size_t i = 0; i < computers_count; ++i) {
    computers_.push_back(new ComputeWorker(scheduler_));
  }

  for (std::size_t i = 0; i < generators_count; ++i) {
    generators_.push_back(new GeneratorWorker(scheduler_, block_pool_));
  }
}

void Controller::Start() {
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

void Controller::Join() {
  for (std::size_t i = 0; i < generators_.size(); ++i) {
    generators_[i].Join();
  }

  for (std::size_t i = 0; i < computers_.size(); ++i) {
    computers_[i].Join();
  }

  scheduler_.Join();
}
