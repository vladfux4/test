#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdint.h>
#include <cstddef>
#include <boost/ptr_container/ptr_vector.hpp>

#include "block_pool.h"
#include "scheduler_worker.h"
#include "generator_worker.h"
#include "compute_worker.h"

class Controller {
 public:
  Controller(const std::size_t block_size,
             const std::size_t blocks_count,
             const std::size_t generators_count,
             const std::size_t computers_count);

  void Start();

  void Join();

 private:
  BlockPool block_pool_;
  SchedulerWorker scheduler_;
  boost::ptr_vector<ComputeWorker> computers_;
  boost::ptr_vector<GeneratorWorker> generators_;
};

#endif // CONTROLLER_H_
