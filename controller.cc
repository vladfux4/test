#include <glog/logging.h>
#include "controller.h"

const std::size_t kBlockSize = 8;
const std::size_t kGeneratorsCount = 4;
const std::size_t kComputersCount = 4;
const std::size_t kBlockCount = 1000;

Controller::Controller()
    : block_pool_(kBlockSize),
      scheduler_(generators_, computers_, kBlockCount),
      computers_(),
      generators_() {
  for (std::size_t i = 0; i < kComputersCount; ++i) {
    computers_.push_back(new ComputeWorker(scheduler_));
  }

  for (std::size_t i = 0; i < kGeneratorsCount; ++i) {
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
