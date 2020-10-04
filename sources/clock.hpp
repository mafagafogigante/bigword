#pragma once

#include <chrono>
#include <string>

namespace BigWord {
class TimePoint {
 public:
  std::chrono::high_resolution_clock::time_point now;
  TimePoint();
};

class Duration {
  std::chrono::high_resolution_clock::duration duration{};

 public:
  Duration(TimePoint begin, TimePoint end);
  double to_nanoseconds() const;
};

void write_time_with_unit(uint64_t number, const std::string& name);

void write_time(double nanoseconds);
}  // namespace BigWord
