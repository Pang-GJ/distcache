#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

using KeyType = std::string;

template <typename ValueType>
struct DefaultSizeOf {
  auto operator()(const ValueType &value) -> size_t { return sizeof(value); }
};

template <typename ValueType, typename SizeOf = DefaultSizeOf<ValueType>>
class LRUCache {
 public:
  // max_bytes==0 means no limit size
  explicit LRUCache(int64_t max_bytes = 0) : max_bytes_(max_bytes) {
    head_ = new Entry();
    tail_ = new Entry();
    head_->next_ = tail_;
    tail_->prev_ = head_;
  }

  ~LRUCache() {
    auto entry = head_->next_;
    while (entry != tail_) {
      auto tmp = entry;
      entry = entry->next_;
      delete tmp;
    }
    delete head_;
    delete tail_;
  }

  struct Entry {
    Entry() = default;
    Entry(KeyType key, const ValueType &value)
        : key_(std::move(key)), value_(value), prev_(nullptr), next_(nullptr) {}

    KeyType key_;
    ValueType value_;
    Entry *prev_;
    Entry *next_;
  };

  // value是出参
  auto Get(const KeyType &key, ValueType *value) -> bool {
    if (hashmap_.count(key) == 0) {
      return false;
    }
    auto entry = hashmap_[key];
    *value = entry->value_;
    MoveToFront(entry);
    return true;
  }

  void Add(const KeyType &key, const ValueType &value) {
    if (hashmap_.count(key) != 0) {
      // 存在
      auto entry = hashmap_[key];
      bytes_ += get_size_functor_(value) - get_size_functor_(entry->value_);
      entry->value_ = value;
      MoveToFront(entry);
    } else {
      // 不存在
      auto entry = PushToFront(key, value);
      bytes_ += get_size_functor_(value) + key.size();
      hashmap_[key] = entry;
    }

    while (max_bytes_ != 0 && max_bytes_ < bytes_) {
      Evicte();
    }
  }

  auto Size() const -> size_t { return hashmap_.size(); }

 private:
  void Evicte() {
    auto last = GetLast();
    if (last == head_) {
      return;
    }
    hashmap_.erase(last->key_);
    bytes_ -= get_size_functor_(last->value_) + last->key_.size();
    last->prev_->next_ = last->next_;
    last->next_->prev_ = last->prev_;
    delete last;
  }

  void MoveToFront(Entry *entry) {
    if (entry->prev_ != nullptr && entry->next_ != nullptr) {
      entry->prev_->next_ = entry->next_;
      entry->next_->prev_ = entry->prev_;
    }
    auto old_first = head_->next_;
    entry->prev_ = head_;
    entry->next_ = old_first;
    old_first->prev_ = entry;
    head_->next_ = entry;
  }

  auto PushToFront(const KeyType &key, const ValueType &value) -> Entry * {
    auto entry = new Entry(key, value);
    MoveToFront(entry);
    return entry;
  }

  auto GetLast() const -> Entry * { return tail_->prev_; }

  Entry *head_;
  Entry *tail_;
  std::unordered_map<KeyType, Entry *> hashmap_;
  int64_t max_bytes_;
  int64_t bytes_{0};  // bytes不包括std::string中的额外开销
  SizeOf get_size_functor_; // 函数对象，用于获取value的size
};
