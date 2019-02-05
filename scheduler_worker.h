#ifndef SCHEDULER_WORKER_H_
#define SCHEDULER_WORKER_H_

#include "consumer_worker.h"
#include "block_pool.h"

#include <queue>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/pool/pool_alloc.hpp>

#include "generator_worker.h"
#include "compute_worker.h"

struct ResultEvent {
  SharedBlock block;
  uint32_t crc;
};

class SchedulerWorker : public ConsumerWorker<SharedBlock, ResultEvent> {
 public:
  SchedulerWorker(boost::ptr_vector<GeneratorWorker>& generators,
                  boost::ptr_vector<ComputeWorker>& computers,
                  const size_t block_count);
  virtual ~SchedulerWorker();

  bool AcquireBlock();

  virtual void TakeEvent(const SharedBlock event) override;
  virtual void TakeEvent(const ResultEvent event) override;
  virtual void ProcessEvents() override;

 private:

  struct BlockData {
    using ResultData = std::vector<uint32_t, boost::fast_pool_allocator<uint32_t>>;

    BlockData(SharedBlock block, std::size_t result_count)
        : block(block), results(),
          kRequiredResultCount(result_count) {
      results.reserve(result_count);
    }

    SharedBlock block;
    ResultData results;
    const size_t kRequiredResultCount;
  };

  struct ResultData {
    SharedBlock block;
    uint32_t crc;
  };

  void ProcessNewBlocks();
  void ProcessResults();
  bool CheckResults(const BlockData& data);
  bool StoreBrokenBlock(const BlockData& data);
  void CheckDoneCondition();

  std::string SerializeResults(const BlockData::ResultData& results);

  const std::size_t kRequiredBlockCount;
  std::size_t generated_block_count_;
  std::size_t processed_new_block_count_;
  std::size_t processed_result_count_;
  boost::mutex generated_block_count_mutex_;

  std::queue<SharedBlock, std::deque<SharedBlock, boost::fast_pool_allocator<SharedBlock>>> new_blocks_;
  std::queue<ResultData, std::deque<ResultData, boost::fast_pool_allocator<ResultData>>> results_;
  std::list<BlockData, boost::fast_pool_allocator<BlockData>> blocks_;

  boost::ptr_vector<GeneratorWorker>& generators_;
  boost::ptr_vector<ComputeWorker>& computers_;
};

#endif // SCHEDULER_WORKER_H_
