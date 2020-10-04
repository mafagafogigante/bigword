#include "data.hpp"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace BigWord {
static constexpr size_t limit_size = std::numeric_limits<uint32_t>::max();
static constexpr char separator = ':';

using std::istream;
using std::ostream;

void dump_safe_string(ostream &os, const std::string &string) {
  os << string << '\n';
}

void dump_unsafe_string(ostream &os, const std::string &string) {
  if (string.size() > limit_size) {
    throw std::runtime_error("String is largest than the size limit.");
  }
  os << string.size() << separator << string << '\n';
}

std::string read_safe_string(istream &is) {
  std::string string;
  is >> string;
  return string;
}

std::string read_unsafe_string(istream &is) {
  size_t size = 0;
  is >> size;
  is.ignore();
  std::string string;
  string.resize(size);
  is.read(&string[0], size);
  return string;
}
}  // namespace BigWord
