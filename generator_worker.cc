#include "generator_worker.h"
#include "scheduler_worker.h"

GeneratorWorker::GeneratorWorker(SchedulerWorker &consumer, BlockPool &pool)
    : consumer_(consumer),
      pool_(pool) {
  static size_t s_id = 0;
  id_ = s_id++;
}

GeneratorWorker::~GeneratorWorker() {
}

void GeneratorWorker::Routine() {
  while (true == consumer_.AcquireBlock()) {
    auto block = pool_.CreateBlock();
    SchedulerEvent event(block);

    for (std::size_t i = 0; i < block.length; ++i) {
      block.data[i] = static_cast<uint8_t>(rand() % 256);
    }

    VLOG(1) << "Thr:" << id_ << " Generate Block: " << SerializeBlock(block);
    consumer_.PushEvent(event);
  }
}
