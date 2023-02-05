#pragma once

#include <memory>
#include <shared_mutex>
#include "lru.h"

template <typename ValueType>
class Cache {
 public:
  explicit Cache(int64_t cache_bytes = 0) : cache_bytes_(cache_bytes) {}

  ~Cache() { delete lru_; }

  void Add(const KeyType &key, const ValueType &value) {
    std::unique_ptr<std::shared_mutex> lock(&mtx_);
    if (lru_ == nullptr) {
      lru_ = new LRU<ValueType>(cache_bytes_);
    }
    lru_->Add(key, value);
  }

  bool Get(const KeyType &key, ValueType *value) {
    std::shared_lock<std::shared_mutex> lock(mtx_);
    if (lru_ == nullptr) {
      return false;
    }
    return lru_->Get(key, value);
  }

 private:
  std::shared_mutex mtx_;
  LRU<ValueType> *lru_;
  int64_t cache_bytes_{0};
};
