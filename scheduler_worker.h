#ifndef SCHEDULER_WORKER_H_
#define SCHEDULER_WORKER_H_

#include "consumer_worker.h"
#include "block_pool.h"

#include <queue>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>

#include "generator_worker.h"
#include "compute_worker.h"

struct SchedulerEvent {
  enum class Type {
    NEW_BLOCK,
    CRC,
  };

  SchedulerEvent(const SharedBlock block)
      : type(Type::NEW_BLOCK), block(block), crc(0) {}

  SchedulerEvent(const SharedBlock block, const uint32_t crc)
      : type(Type::CRC), block(block), crc(crc) {}

  Type type;
  SharedBlock block;
  uint32_t crc;
};

class SchedulerWorker : public ConsumerWorker<SchedulerEvent> {
 public:
  SchedulerWorker(boost::ptr_vector<GeneratorWorker>& generators,
                  boost::ptr_vector<ComputeWorker>& computers);
  virtual ~SchedulerWorker();

  bool AcquireBlock();

  virtual void TakeEvent(const SchedulerEvent event) override;
  virtual void ProcessEvents() override;

 private:

  struct BlockData {
    BlockData(SharedBlock block, std::size_t result_count)
        : block(block), results(),
          kRequiredResultCount(result_count) {
      results.reserve(result_count);
    }

    SharedBlock block;
    std::vector<uint32_t> results;
    const size_t kRequiredResultCount;
  };

  struct ResultData {
    SharedBlock block;
    uint32_t crc;
  };

  const std::size_t kRequiredBlockCount;
  std::size_t generated_block_count_;
  std::size_t processed_new_block_count_;
  std::size_t processed_result_count_;
  boost::mutex generated_block_count_mutex_;

  std::queue<SharedBlock> new_blocks_;
  std::queue<ResultData> results_;
  std::list<BlockData> blocks_;

  boost::ptr_vector<GeneratorWorker>& generators_;
  boost::ptr_vector<ComputeWorker>& computers_;
};

#endif // SCHEDULER_WORKER_H_
