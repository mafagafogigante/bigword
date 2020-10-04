#include "rules.hpp"

#include <algorithm>
#include <string>

namespace BigWord {
bool is_valid_word(const std::string &word) noexcept {
  if (word.empty()) {
    return false;
  }
  return std::all_of(std::begin(word), std::end(word), is_valid_letter);
}
}  // namespace BigWord