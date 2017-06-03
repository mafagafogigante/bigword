#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <cstdint>
#include "rules.hpp"

/**
 * An analysis of the most efficient way to compare two letter counts.
 *
 * Analyze the words read to later improve filtering speed.
 */
class Analysis {
 private:
  // How many words have at least one occurrence of this letter.
  uint32_t words = 0;
  uint32_t word_count[alphabet_size] = {0};
  size_t best_order[alphabet_size] = {0};

  void find_best_order();

 public:
  void analyze(const std::string word);
  void compile();
  size_t best_index(const size_t comparison) const;

  friend std::ostream &operator<<(std::ostream &os, const Analysis &analysis);
  friend std::istream &operator>>(std::istream &is, Analysis &analysis);
};

#endif
