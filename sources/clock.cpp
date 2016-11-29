#include "clock.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

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

static const double thousand = 1000.0;
static const double million = thousand * thousand;
static const double billion = thousand * thousand * thousand;

void write_time_with_unit(const uint64_t number, std::string name) {
  if (number == 1) {
    std::cout << number << ' ' << name;
  } else {
    std::cout << number << ' ' << name << 's';
  }
}

void write_time(const double nanoseconds) {
  if (nanoseconds >= billion) {
    const double seconds = nanoseconds / billion;
    write_time_with_unit(std::floor(seconds), "second");
  } else if (nanoseconds >= million) {
    const double milliseconds = nanoseconds / million;
    write_time_with_unit(std::floor(milliseconds), "millisecond");
  } else if (nanoseconds >= thousand) {
    const double microseconds = nanoseconds / thousand;
    write_time_with_unit(std::floor(microseconds), "microsecond");
  } else {
    write_time_with_unit(std::floor(nanoseconds), "nanosecond");
  }
}