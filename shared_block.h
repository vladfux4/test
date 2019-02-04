#ifndef SHARED_BLOCK_H_
#define SHARED_BLOCK_H_

#include "static_pool_alloc.h"
#include <sstream>

struct SharedBlock {
  using BlockItem = uint8_t;
  using BlockData = BlockItem[];

  boost::shared_ptr<BlockData> data;
  std::size_t length;
};

inline std::string SerializeBlock(const SharedBlock task) {
  std::stringstream ss;
  for (std::size_t i = 0; i < task.length; ++i) {
    ss << std::hex << static_cast<int>(task.data[i]) << " ";
  }

  return ss.str();
}

#endif // SHARED_BLOCK_H_
