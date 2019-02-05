#include "compute_worker.h"
#include "scheduler_worker.h"
#include "crc32_calculator.h"

// result could be compared at https://crccalc.com/
struct Mpeg2CRC32 {
  inline static uint32_t Calculate(const uint8_t *buf, uint32_t len) {
    return CRC32Calculator<0X04C11DB7>::Calculate(buf, len, 0XFFFFFFFF);
  }
};

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
    auto crc = Mpeg2CRC32::Calculate(&block.data[0], block.length);

#ifdef ENABLE_EVIL_ELECTRIC_FIELD
    static uint32_t error_index = 0;
    if (0 == (error_index % 100)) { crc = 0XFFFFFFFF; }
    error_index++;
#endif // ENABLE_EVIL_ELECTRIC_FIELD

    ResultEvent event{block, crc};
    scheduler_.PushEvent(event);
  }
}
