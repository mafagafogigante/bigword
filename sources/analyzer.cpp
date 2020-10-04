#include "analyzer.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

#include "rules.hpp"

namespace BigWord {
void Analysis::analyze(const std::string &word) {
  words++;
  for (const char letter : word) {
    if (is_valid_letter(letter)) {
      word_count[get_letter_index(letter)]++;
    }
  }
}

void Analysis::compile() {
  find_best_order();
}

struct IndexScore {
  size_t index = 0;
  double score = 0;

  bool operator<(const IndexScore &other) const {
    return score < other.score;
  }
};

/**
 * Determines the best comparison order for letter frequencies based on data.
 *
 * This function runs in O(n) with respect to alphabet size.
 */
void Analysis::find_best_order() {
  // # Original Reasoning
  // Characters which are present in most words do not help.
  // Characters which are absent in most words do not help.
  // Characters which are present in about half the words are best.
  //
  // # Results
  // Empirically, I was wrong. Common characters improve comparison performance!
  //
  // # Formula
  // Score = frequency

  // Avoid divisions by zero.
  if (words == 0) {
    return;
  }
  std::array<IndexScore, AlphabetSize> scores;
  for (size_t i = 0; i < AlphabetSize; i++) {
    scores[i].index = i;
    scores[i].score = word_count[i] / static_cast<double>(words);
  }
  std::sort(std::rbegin(scores), std::rend(scores));
  for (size_t i = 0; i < AlphabetSize; i++) {
    best_order[i] = scores[i].index;
  }
}

size_t Analysis::best_index(const size_t comparison) const {
  return best_order[comparison];
}

std::ostream &operator<<(std::ostream &os, const Analysis &analysis) {
  os << analysis.words;
  os << '\n';
  for (size_t i = 0; i < AlphabetSize; i++) {
    os << analysis.word_count[i];
    os << '\n';
  }
  for (size_t i = 0; i < AlphabetSize; i++) {
    os << analysis.best_order[i];
    if (i + 1 != AlphabetSize) {
      os << '\n';
    }
  }
  return os;
}

std::istream &operator>>(std::istream &is, Analysis &analysis) {
  is >> analysis.words;
  for (size_t i = 0; i < AlphabetSize; i++) {
    is >> analysis.word_count[i];
  }
  for (size_t i = 0; i < AlphabetSize; i++) {
    is >> analysis.best_order[i];
  }
  return is;
}
}  // namespace BigWord