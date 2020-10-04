#pragma once

#include <array>
#include <cstdint>

#include "rules.hpp"

namespace BigWord {
/**
 * An analysis of the most efficient way to compare two letter counts.
 *
 * Analyze the words read to later improve filtering speed.
 */
class Analysis {
 private:
  // How many words have been analyzed.
  uint32_t words = 0;
  // How many words have at least one occurrence of this character.
  std::array<std::uint32_t, AlphabetSize> word_count{};
  std::array<std::size_t, AlphabetSize> best_order{};

  void find_best_order();

 public:
  void analyze(const std::string &word);
  void compile();
  /**
   * Returns the best index to use for the n-th comparison.
   */
  [[nodiscard]] std::size_t best_index(std::size_t comparison) const;

  friend std::ostream &operator<<(std::ostream &os, const Analysis &analysis);
  friend std::istream &operator>>(std::istream &is, Analysis &analysis);
};
}  // namespace BigWord