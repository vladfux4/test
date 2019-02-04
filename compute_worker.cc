#include "compute_worker.h"

#include "scheduler_worker.h"

ComputeWorker::ComputeWorker(SchedulerWorker& scheduler)
    : scheduler_(scheduler) {
}

ComputeWorker::~ComputeWorker() {
}

void ComputeWorker::TakeEvent(const SharedBlock event) {
  blocks_.push(event);
}

void ComputeWorker::ProcessEvents() {
  while (false == blocks_.empty()) {
    auto block = blocks_.front();
    blocks_.pop();

    VLOG(1) << "Calculate Block: " << SerializeBlock(block);

    SchedulerEvent event(block, 0xFF);
    scheduler_.PushEvent(event);
  }
}
