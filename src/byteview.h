#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>

struct ByteView {
 public:
  ByteView() = default;
  explicit ByteView(std::string_view view) : view_(view), size_(view.size()) {}

  size_t Size() const { return size_; }

  std::string_view String() const { return view_; }

  std::string ByteSlice() { return view_.data(); }

 private:
  std::string_view view_;
  size_t size_;
};
