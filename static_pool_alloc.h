#ifndef STATIC_POOL_ALLOC_H_
#define STATIC_POOL_ALLOC_H_

#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <boost/pool/pool.hpp>
#include <boost/thread/mutex.hpp>
#include <glog/logging.h>

template <class T, class Tag>
class StaticPoolAllocator {
 public:
  // allocator standart types definition
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  // memory storage data type
  using Pool = boost::pool<>;

  static StaticPoolAllocator& GetInstance() {
    static StaticPoolAllocator alloc;
    return alloc;
  }

  // return address of values
  pointer address(reference value) const {
    return &value;
  }

  const_pointer address(const_reference value) const {
    return &value;
  }

  StaticPoolAllocator(const StaticPoolAllocator&) {
  }

  template <class U>
  StaticPoolAllocator(const StaticPoolAllocator<U, Tag>&) {
  }

  ~StaticPoolAllocator() {
  }

  static void Init(const std::size_t block_size) {
    static bool init = false;
    if (false == init) {
      new (GetPool()) Pool(block_size);
      init = true;
    }
  }

  static void Destroy() {
    GetPool()->~Pool();
  }

  // return maximum number of elements that can be allocated
  size_type max_size() const {
    return std::numeric_limits<std::size_t>::max() / sizeof(T);
  }

  // allocate but don't initialize num elements of type T
  pointer allocate(size_type num, const void* = 0) {
    pointer ret_val = nullptr;
    const std::size_t kBlockSize = num * sizeof(T);

    Init(kBlockSize);
    if (IsRequestValid(kBlockSize)) {
      LockPool(true);
      ret_val = reinterpret_cast<T*>(GetPool()->malloc());
      LockPool(false);

      VLOG(2) << "allocate from pool: " << kBlockSize
              << " ptr: " << reinterpret_cast<void*>(ret_val);
    } else {
      assert(false);
    }

    return ret_val;
  }

  // deallocate storage p of deleted elements
  void deallocate(pointer p, size_type num) {
    const std::size_t kBlockSize = num * sizeof(T);
    if (IsRequestValid(kBlockSize)) {
      VLOG(2) << "deallocate from pool " << kBlockSize
              << " ptr: " << reinterpret_cast<void*>(p);

      LockPool(true);
      GetPool()->free(p);
      LockPool(false);
    } else {
      assert(false);
    }

  }

 private:
  StaticPoolAllocator() {
  }

  static Pool* GetPool() {
    static uint8_t storage[sizeof(Pool)];
    return reinterpret_cast<Pool*>(storage);
  }

  static void LockPool(const bool state) {
    static boost::mutex mutex;
    if (true == state) {
      mutex.lock();
    } else {
      mutex.unlock();
    }
  }

  static inline bool IsRequestValid(const std::size_t size) {
    return (size == GetPool()->get_requested_size());
  }
};

#endif // STATIC_POOL_ALLOC_H_
