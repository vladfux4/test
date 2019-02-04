#ifndef COMPUTE_WORKER_H_
#define COMPUTE_WORKER_H_

#include "consumer_worker.h"
#include "shared_block.h"

class SchedulerWorker;

class ComputeWorker : public ConsumerWorker<SharedBlock> {
 public:
  ComputeWorker(SchedulerWorker& scheduler);
  virtual ~ComputeWorker();

  virtual void TakeEvent(const SharedBlock event) override;
  virtual void ProcessEvents() override;

 private:
  SchedulerWorker& scheduler_;
  std::queue<SharedBlock> blocks_;
};

#endif // COMPUTE_WORKER_H_
