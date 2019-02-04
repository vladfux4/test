#ifndef BLOCK_POOL_H_
#define BLOCK_POOL_H_

#include <boost/smart_ptr/allocate_shared_array.hpp>

#include "static_pool_alloc.h"
#include "shared_block.h"

class BlockPool {
 public:
  struct AllocatorTag;
  using Allocator = StaticPoolAllocator<SharedBlock::BlockItem, AllocatorTag>;

  BlockPool(const std::size_t block_length)
      : kBlockLength(block_length) {
     Allocator::Init(kBlockLength);
  }

  ~BlockPool() {
    Allocator::Destroy();
  }

  SharedBlock CreateBlock() {
    return SharedBlock{
        boost::allocate_shared<SharedBlock::BlockData, Allocator>(
            Allocator::GetInstance(), kBlockLength),
        kBlockLength};
  }

 private:
  const std::size_t kBlockLength;
};

#endif // BLOCK_POOL_H_
