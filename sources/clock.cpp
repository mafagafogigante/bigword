#include "clock.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

namespace BigWord {
TimePoint::TimePoint() {
  now = std::chrono::high_resolution_clock::now();
}

Duration::Duration(TimePoint begin, TimePoint end) {
  duration = end.now - begin.now;
}

double Duration::to_nanoseconds() const {
  auto us = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
  return us.count();
}

static constexpr double thousand = 1000.0;
static constexpr double million = thousand * thousand;
static constexpr double billion = thousand * thousand * thousand;

void write_time_with_unit(const uint64_t number, const std::string& name) {
  std::cout << number << ' ' << name;
  if (number != 1) {
    std::cout << 's';
  }
}

void write_time(const double nanoseconds) {
  if (nanoseconds >= billion) {
    const auto seconds = nanoseconds / billion;
    write_time_with_unit(std::floor(seconds), "second");
  } else if (nanoseconds >= million) {
    const auto milliseconds = nanoseconds / million;
    write_time_with_unit(std::floor(milliseconds), "millisecond");
  } else if (nanoseconds >= thousand) {
    const auto microseconds = nanoseconds / thousand;
    write_time_with_unit(std::floor(microseconds), "microsecond");
  } else {
    write_time_with_unit(std::floor(nanoseconds), "nanosecond");
  }
}
}  // namespace BigWord
