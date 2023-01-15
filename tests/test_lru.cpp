#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <fmt/core.h>
#include <memory>
#include <string>
#include "lru.h"

struct GetStringSize {
  GetStringSize() = default;
  auto operator()(const std::string &value) -> size_t { return value.size(); }
};

TEST_CASE("test get, add") {
  auto lru = std::make_unique<LRUCache<std::string, GetStringSize>>();
  lru->Add("key1", "1234");
  lru->Add("key2", "5678");
  std::string res;
  auto ok = lru->Get("key1", &res);
  CHECK_EQ(ok, true);
  CHECK_EQ(res, "1234");
  ok = lru->Get("key2", &res);
  CHECK_EQ(ok, true);
  CHECK_EQ(res, "5678");
}

TEST_CASE("test evicte") {
  std::string key1("key1");
  std::string key2("key2");
  std::string key3("key3");
  std::string val1("123");
  std::string val2("456");
  std::string val3("789");
  auto cap = key1.size() + key2.size() + val1.size() + val2.size();
  auto lru = std::make_unique<LRUCache<std::string, GetStringSize>>(cap);
  lru->Add(key1, val1);
  lru->Add(key2, val2);
  lru->Add(key3, val3);

  std::string res;
  auto ok = lru->Get("key1", &res);
  CHECK_EQ(ok, false);
  CHECK_EQ(lru->Size(), 2);
}
