#ifndef GENERATOR_WORKER_H_
#define GENERATOR_WORKER_H_

#include "worker.h"
#include "block_pool.h"

class SchedulerWorker;

class GeneratorWorker : public Worker {
 public:
  GeneratorWorker(SchedulerWorker& consumer, BlockPool& pool);

  virtual ~GeneratorWorker();

  virtual void Routine() override;

 private:
  SchedulerWorker& consumer_;
  BlockPool& pool_;
  size_t id_;
};

#endif // GENERATOR_WORKER_H_
