#include "scheduler_worker.h"
#include <sstream>

SchedulerWorker::SchedulerWorker(boost::ptr_vector<GeneratorWorker>& generators,
                                 boost::ptr_vector<ComputeWorker>& computers,
                                 const size_t block_count)
    : kRequiredBlockCount(block_count),
      generated_block_count_(0),
      processed_new_block_count_(0),
      processed_result_count_(0),
      generated_block_count_mutex_(),
      new_blocks_(),
      results_(),
      blocks_(),
      generators_(generators),
      computers_(computers) {
}

SchedulerWorker::~SchedulerWorker() {
}

bool SchedulerWorker::AcquireBlock() {
  boost::lock_guard<boost::mutex> lock(generated_block_count_mutex_);
  bool ret_val = false;

  if (generated_block_count_ < kRequiredBlockCount) {
    generated_block_count_++;
    ret_val = true;
  }

  return ret_val;
}

void SchedulerWorker::TakeEvent(const SchedulerEvent event) {
  if (SchedulerEvent::Type::NEW_BLOCK == event.type) {
    new_blocks_.push(event.block);
  } else if (SchedulerEvent::Type::CRC == event.type) {
    results_.push({event.block, event.crc});
  }
}

void SchedulerWorker::ProcessEvents() {
  ProcessNewBlocks();
  ProcessResults();
  CheckDoneCondition();
}

void SchedulerWorker::ProcessNewBlocks() {
  while (false == new_blocks_.empty()) {
    auto block = new_blocks_.front();
    new_blocks_.pop();
    processed_new_block_count_++;

    VLOG(1) << "Process New Block: " << SerializeBlock(block);
    blocks_.push_back(BlockData(block, computers_.size()));
    for (std::size_t i = 0; i < computers_.size(); ++i) {
      computers_[i].PushEvent(block);
    }
  }
}

void SchedulerWorker::ProcessResults() {
  while (false == results_.empty()) {
    auto result = results_.front();
    results_.pop();
    processed_result_count_++;

    VLOG(1) << "Process Result: "
            << SerializeBlock(result.block)
            << " CRC: " << std::hex << result.crc;

    //find in list
    auto block_it = blocks_.begin();
    for (; block_it != blocks_.end(); ++block_it) {
      if (block_it->block.data.get() == result.block.data.get()) {
        break;
      }
    }

    if (blocks_.end() == block_it) {
      LOG(ERROR) << "Block was not found";
    } else {
      block_it->results.push_back(result.crc);
      VLOG(1) << "Stored CRC:" << result.crc << " result_count:" << block_it->results.size();

      if (block_it->results.size() == block_it->kRequiredResultCount) {
        if (false == CheckResults(*block_it)) {
          StoreBrokenBlock(*block_it);
        }

        blocks_.erase(block_it);
      }
    }
  }
}

bool SchedulerWorker::CheckResults(const SchedulerWorker::BlockData& data) {
  VLOG(1) << "Check block: " << SerializeBlock(data.block)
          << " CRC: " << SerializeResults(data.results);

  bool status = true;
  auto prev = data.results[0];
  for (std::size_t i = 1; i < data.results.size(); ++i) {
    if (data.results[i] != prev) {
      status = false;
      break;
    }

    prev = data.results[i];
  }

  return status;
}

bool SchedulerWorker::StoreBrokenBlock(const SchedulerWorker::BlockData& data) {
  LOG(ERROR) << "WRONG RESUT!";
}

void SchedulerWorker::CheckDoneCondition() {
  VLOG(1) << "Processed: {new blocks:"
            << processed_new_block_count_
            << ", results:"
            << processed_result_count_
            << "}";

  if ((kRequiredBlockCount == processed_new_block_count_) &&
      ((kRequiredBlockCount * computers_.size()) == processed_result_count_)) {
    StopListen();
    for (std::size_t i = 0; i < computers_.size(); ++i) {
      computers_[i].NotifyStopListen();
    }
  }
}

std::string SchedulerWorker::SerializeResults(const BlockData::ResultData& results) {
  std::stringstream ss;

  for (std::size_t i = 0; i < results.size(); ++i) {
    ss << std::hex << results[i] << " ";
  }

  return ss.str();
}
